#include "portal.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/pbuf.h"
#include "esp_log.h"
#include "esp_netif.h"
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "lwip/lwip_napt.h"
#include "esp_timer.h"

static const char *TAG = "DNSHIJACK";

static struct udp_pcb *pcb = NULL;
static ip4_addr_t s_ap_ip;
extern esp_netif_t *wifiSTA;

static esp_timer_handle_t s_nat_timer = NULL;
static bool s_nat_active = false;
static portMUX_TYPE s_nat_lock = portMUX_INITIALIZER_UNLOCKED;

static bool enable_nat_for_ap(void)
{
    bool ok = false;
    portENTER_CRITICAL(&s_nat_lock);
    if (s_nat_active) {
        ok = true;
        portEXIT_CRITICAL(&s_nat_lock);
        return ok;
    }

    esp_netif_ip_info_t ip_info;
    esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (!ap_netif) {
        ESP_LOGW(TAG, "enable_nat_for_ap: no AP netif");
        portEXIT_CRITICAL(&s_nat_lock);
        return false;
    }
    if (esp_netif_get_ip_info(ap_netif, &ip_info) != ESP_OK) {
        ESP_LOGW(TAG, "enable_nat_for_ap: get_ip_info failed");
        portEXIT_CRITICAL(&s_nat_lock);
        return false;
    }

    ip_napt_enable(ip_info.ip.addr, 1);
    s_nat_active = true;
    ok = true;
    portEXIT_CRITICAL(&s_nat_lock);
    ESP_LOGI(TAG, "NAT enabled for AP (ip=%s)", ip4addr_ntoa((ip4_addr_t*)&ip_info.ip));
    return ok;
}

static void disable_nat_for_ap(void)
{
    portENTER_CRITICAL(&s_nat_lock);
    if (!s_nat_active) {
        portEXIT_CRITICAL(&s_nat_lock);
        return;
    }
    esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (ap_netif) {
        esp_netif_ip_info_t ip_info;
        if (esp_netif_get_ip_info(ap_netif, &ip_info) == ESP_OK)
            ip_napt_enable(ip_info.ip.addr, 0);
    }
    s_nat_active = false;
    portEXIT_CRITICAL(&s_nat_lock);
    ESP_LOGI(TAG, "NAT disabled for AP");
}

static void nat_timer_cb(void *arg)
{
    (void)arg;
    disable_nat_for_ap();
    if (s_nat_timer) {
        esp_timer_stop(s_nat_timer);
        esp_timer_delete(s_nat_timer);
        s_nat_timer = NULL;
    }
}

static void enable_nat_temporarily(int sec)
{
    if (sec <= 0) return;
    if (!enable_nat_for_ap()) return;

    if (s_nat_timer) {
        esp_timer_stop(s_nat_timer);
        esp_timer_delete(s_nat_timer);
        s_nat_timer = NULL;
    }
    const esp_timer_create_args_t args = {.callback = &nat_timer_cb, .name = "nat_timer"};
    if (esp_timer_create(&args, &s_nat_timer) == ESP_OK) {
        esp_timer_start_once(s_nat_timer, (int64_t)sec * 1000000LL);
        ESP_LOGI(TAG, "NAT will auto-disable in %d sec", sec);
    }
}

void dns_set_nat_open(bool on)
{
    if (on) enable_nat_for_ap();
    else disable_nat_for_ap();
}

static bool is_whitelisted_domain_name(const char *q)
{
    if (!q || !*q) return false;

    char name[256];
    int i;
    for (i = 0; q[i] && i < sizeof(name)-1; ++i) {
        char c = q[i];
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
        name[i] = c;
    }
    name[i] = '\0';

    if (strstr(name, "line.me")) return true;
    if (strstr(name, "line-scdn.net")) return true;
    if (strstr(name, "lineapp.me")) return true;
    if (strstr(name, "liff.line.me")) return true;

    if (strstr(name, "bilgier-unabstractively-justine.ngrok-free.dev")) return true;
    if (strstr(name, "ngrok-free.dev")) return true;

    return false;
}

static bool get_upstream_dns_ip(ip4_addr_t *out)
{
    if (!out) return false;
    esp_netif_dns_info_t di;
    if (esp_netif_get_dns_info(wifiSTA, ESP_NETIF_DNS_MAIN, &di) == ESP_OK &&
        di.ip.type == ESP_IPADDR_TYPE_V4)
        out->addr = di.ip.u_addr.ip4.addr;
    else
        out->addr = inet_addr("8.8.8.8");
    return true;
}

typedef struct {
    ip_addr_t addr;
    u16_t port;
    uint16_t len;
    uint8_t buf[512];
} dns_job_t;

static QueueHandle_t s_dns_q = NULL;

static void enqueue_forward(const ip_addr_t *addr, u16_t port, const struct pbuf *p)
{
    if (!s_dns_q || !p) return;
    dns_job_t job = {0};
    job.addr = *addr;
    job.port = port;
    uint16_t n = p->tot_len;
    if (n > sizeof(job.buf)) n = sizeof(job.buf);
    pbuf_copy_partial((struct pbuf*)p, job.buf, n, 0);
    job.len = n;
    xQueueSend(s_dns_q, &job, 0);
}

static void dns_fw_task(void *arg)
{
    ip4_addr_t up;
    get_upstream_dns_ip(&up);
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { ESP_LOGE(TAG, "DNS forward socket failed"); vTaskDelete(NULL); return; }

    struct timeval tv = {.tv_sec = 1, .tv_usec = 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    if (wifiSTA) {
        int if_index = esp_netif_get_netif_impl_index(wifiSTA);
        if (if_index >= 0)
            setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, &if_index, sizeof(if_index));
    }

    struct sockaddr_in up_dns = {.sin_family = AF_INET, .sin_port = htons(53)};
    up_dns.sin_addr.s_addr = up.addr;
    ESP_LOGI(TAG, "DNS upstream = %s", inet_ntoa(*(struct in_addr*)&up.addr));

    for (;;) {
        dns_job_t job;
        if (xQueueReceive(s_dns_q, &job, portMAX_DELAY) != pdTRUE) continue;
        sendto(sock, job.buf, job.len, 0, (struct sockaddr*)&up_dns, sizeof(up_dns));

        uint8_t rbuf[512];
        struct sockaddr_in from; socklen_t flen = sizeof(from);
        int n = recvfrom(sock, rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &flen);
        if (n > 0) {
            struct pbuf *resp = pbuf_alloc(PBUF_TRANSPORT, n, PBUF_RAM);
            if (!resp) continue;
            memcpy(resp->payload, rbuf, n);
            udp_sendto(pcb, resp, &job.addr, job.port);
            pbuf_free(resp);
        }
    }
}

static int parse_dns_query_name(const uint8_t *payload, int len, char *out, int out_len)
{
    if (!payload || len <= 12 || !out || out_len <= 0) return -1;
    int pos = 12, out_pos = 0, jumped = 0;
    while (pos < len) {
        uint8_t slen = payload[pos];
        if (slen == 0) { out[out_pos] = '\0'; return 0; }
        if ((slen & 0xC0) == 0xC0) { out[out_pos] = '\0'; return 0; }
        pos++;
        if (pos + slen > len) break;
        if (out_pos + slen + 1 < out_len) {
            memcpy(&out[out_pos], &payload[pos], slen);
            out_pos += slen; out[out_pos++]='.';
        } else { out[out_pos]='\0'; return 0; }
        pos += slen;
        if (++jumped > 10) break;
    }
    if (out_len>0) out[out_len-1]='\0';
    return -1;
}

static void dns_recv(void *arg, struct udp_pcb *upcb,
                     struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if (!p) return;

    char qname[256] = {0};
    parse_dns_query_name((const uint8_t*)p->payload, p->len, qname, sizeof(qname));
    ESP_LOGI(TAG, "DNS query: %s", qname);

    if (s_nat_active) {
        enqueue_forward(addr, port, p);
        pbuf_free(p);
        return;
    }

    if (portal_is_logged_in(NULL)) {
        enqueue_forward(addr, port, p);
        pbuf_free(p);
        return;
    }

    if (is_whitelisted_domain_name(qname)) {
        enable_nat_temporarily(180);
        enqueue_forward(addr, port, p);
        pbuf_free(p);
        return;
    }

    struct pbuf *resp = pbuf_alloc(PBUF_TRANSPORT, p->tot_len + 32, PBUF_RAM);
    if (resp) {
        memset(resp->payload, 0, resp->tot_len);
        pbuf_copy_partial(p, resp->payload, p->tot_len, 0);
        uint8_t *buf = (uint8_t*)resp->payload;
        buf[2] |= 0x80; buf[3] |= 0x80; buf[6]=0; buf[7]=1;
        int off=12; while(off<p->tot_len && buf[off]!=0){uint8_t l=buf[off]; if((l&0xC0)==0xC0){off+=2;break;} off+=1+l;}
        if (off < p->tot_len && buf[off] == 0) { off++; }
        off += 4;
        buf[off++]=0xC0; buf[off++]=0x0C; buf[off++]=0x00; buf[off++]=0x01;
        buf[off++]=0x00; buf[off++]=0x01; buf[off++]=0x00; buf[off++]=0x00;
        buf[off++]=0x00; buf[off++]=0x3C; buf[off++]=0x00; buf[off++]=0x04;
        uint32_t ip_be=s_ap_ip.addr; memcpy(&buf[off],&ip_be,4); off+=4;
        resp->len=off; resp->tot_len=off;
        udp_sendto(upcb, resp, addr, port);
        pbuf_free(resp);
    }
    pbuf_free(p);
}

void dns_hijack_start(ip4_addr_t ap_ip)
{
    s_ap_ip = ap_ip;

    disable_nat_for_ap(); 
    s_nat_active = false;
    portal_set_logged_in(NULL, false);

    ESP_LOGI(TAG, "DNS hijack start on %s", ip4addr_ntoa(&s_ap_ip));

    if (!s_dns_q) s_dns_q = xQueueCreate(8, sizeof(dns_job_t));
    xTaskCreatePinnedToCore(dns_fw_task, "dns_fw_task", 4096, NULL, 13, NULL, 0);

    pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) return;
    if (udp_bind(pcb, IP_ANY_TYPE, 53) != ERR_OK) { udp_remove(pcb); pcb=NULL; return; }
    udp_recv(pcb, dns_recv, NULL);
    ESP_LOGI(TAG, "DNS hijack running on :53");
}
