#include "portal.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_netif.h"
#include <string.h>
#include "mqtt_notify.h"
#include "esp_wifi.h"
#include "lwip/lwip_napt.h"

static const char *TAG = "PORTAL";
static httpd_handle_t s_server = NULL;
static bool s_logged_in = false;
static esp_timer_handle_t nat_timer = NULL;


static esp_err_t nat_enable_now(void) {
    esp_netif_t *ap = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (!ap) return ESP_FAIL;
    esp_netif_ip_info_t ip;
    if (esp_netif_get_ip_info(ap, &ip) == ESP_OK) {
        ip_napt_enable(ip.ip.addr, 1);
        ESP_LOGI(TAG, "NAT enabled");
        return ESP_OK;
    }
    return ESP_FAIL;
}

static void nat_disable_cb(void *arg) {
    esp_netif_t *ap = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (!ap) return;
    esp_netif_ip_info_t ip;
    if (esp_netif_get_ip_info(ap, &ip) == ESP_OK) {
        ip_napt_enable(ip.ip.addr, 0);
        ESP_LOGI(TAG, "NAT disabled after timeout");
    }
}

void nat_enable_temporarily(int seconds) {
    if (nat_enable_now() != ESP_OK) return;

    if (!nat_timer) {
        const esp_timer_create_args_t args = { .callback = &nat_disable_cb, .name = "nat_temp_timer" };
        esp_timer_create(&args, &nat_timer);
    } else {
        esp_timer_stop(nat_timer);
    }
    esp_timer_start_once(nat_timer, (int64_t)seconds * 1000000LL);
    ESP_LOGI(TAG, "NAT temporarily enabled for %d sec", seconds);
}

bool portal_is_logged_in(const uint8_t mac[6]) { (void)mac; return s_logged_in; }
void portal_set_logged_in(const uint8_t mac[6], bool ok) { (void)mac; s_logged_in = ok; }

static esp_err_t root_get(httpd_req_t *req) {
    ESP_LOGI(TAG, "📲 Root page opened, enabling NAT...");
    nat_enable_temporarily(180);

    const char *html =
        "<!DOCTYPE html><html lang='th'><head>"
        "<meta charset='utf-8'>"
        "<meta name='viewport' content='width=device-width,initial-scale=1.0'>"
        "<title>OffLearn Wi-Fi</title>"
        "<style>"
        "body{font-family:sans-serif;text-align:center;margin-top:60px;}"
        "button{background:#06c755;color:#fff;padding:14px 28px;border:none;"
        "border-radius:8px;font-size:18px;cursor:pointer;}"
        "p{color:#333;font-size:16px;}"
        "</style></head><body>"
        "<h2>เข้าสู่ระบบ Wi-Fi ด้วย LINE</h2>"
        "<p>กดปุ่มด้านล่างเพื่อเข้าสู่ระบบ</p>"
        "<button id='loginBtn'>Login ด้วย LINE</button>"
        "<script>"
        "const LIFF_URL='https://liff.line.me/2008301054-NWkx35W4';"
        "document.getElementById('loginBtn').addEventListener('click',()=>{"
        "  const btn=document.getElementById('loginBtn');"
        "  btn.disabled=true;"
        "  btn.textContent='กำลังเปิด LINE...';"
        "  console.log('🚀 Redirecting to LIFF:', LIFF_URL);"
        "  window.location.href=LIFF_URL;"
        "});"
        "</script>"
        "</body></html>";

    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t login_notify_post(httpd_req_t *req) {
    ESP_LOGI(TAG, "/login_notify method=%d content_len=%d",
             req->method, (int)req->content_len);

    char buf[256];
    int to_read = req->content_len;
    if (to_read > (int)sizeof(buf) - 1) to_read = sizeof(buf) - 1;

    int recvd = 0;
    while (recvd < to_read) {
        int r = httpd_req_recv(req, buf + recvd, to_read - recvd);
        if (r <= 0) {
            if (r == HTTPD_SOCK_ERR_TIMEOUT) continue;
            ESP_LOGW(TAG, "⚠️ httpd_req_recv failed r=%d", r);
            break;
        }
        recvd += r;
    }
    buf[recvd] = '\0';

    char userId[128] = {0};
    char name[128] = {0};

    if (recvd > 0) {
        ESP_LOGI(TAG, "Body: %s", buf);
        sscanf(buf, "{\"userId\":\"%127[^\"]\",\"name\":\"%127[^\"]", userId, name);
    }

    ESP_LOGI(TAG, "Name: %s", name[0]?name:"(unknown)");
    ESP_LOGI(TAG, "UserID: %s", userId[0]?userId:"(unknown)");

    if (userId[0] && name[0]) {
        s_logged_in = true;
        if (nat_enable_now() == ESP_OK)
            ESP_LOGI(TAG, "✅ Backend notified login success, NAT enabled");
        else
            ESP_LOGW(TAG, "NAT enable failed");
        httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    ESP_LOGW(TAG, "Missing userId/name");
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing userId/name");
    return ESP_FAIL;
}

static esp_err_t success_get(httpd_req_t *req) {
    const char *html =
        "<!DOCTYPE html><html><head><meta charset='utf-8'>"
        "<meta name='viewport' content='width=device-width,initial-scale=1.0'>"
        "<title>Connected</title></head>"
        "<body style='text-align:center;margin-top:60px;font-family:sans-serif;'>"
        "<h2>เชื่อมต่อสำเร็จ</h2>"
        "<p>สามารถใช้งานอินเทอร์เน็ตได้แล้ว</p>"
        "<script>setTimeout(()=>window.close(),2000);</script>"
        "</body></html>";
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t captive_probe(httpd_req_t *req) {
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "http://192.168.4.1/");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

void portal_init(esp_netif_t *ap_netif) {
    (void)ap_netif;
    if (s_server) return;

    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    cfg.server_port = 80;
    cfg.max_uri_handlers = 20;

    if (httpd_start(&s_server, &cfg) == ESP_OK) {
        httpd_register_uri_handler(s_server, &(httpd_uri_t){ .uri="/", .method=HTTP_GET, .handler=root_get });
        httpd_register_uri_handler(s_server, &(httpd_uri_t){ .uri="/login_notify", .method=HTTP_POST, .handler=login_notify_post });
        httpd_register_uri_handler(s_server, &(httpd_uri_t){ .uri="/success", .method=HTTP_GET, .handler=success_get });
        httpd_register_uri_handler(s_server, &(httpd_uri_t){ .uri="/generate_204", .method=HTTP_GET, .handler=captive_probe });
        httpd_register_uri_handler(s_server, &(httpd_uri_t){ .uri="/gen_204", .method=HTTP_GET, .handler=captive_probe });
        httpd_register_uri_handler(s_server, &(httpd_uri_t){ .uri="/hotspot-detect.html", .method=HTTP_GET, .handler=captive_probe });

        ESP_LOGI(TAG, "Portal started (LINE LIFF mode)");
        mqtt_notify_start();
    } else {
        ESP_LOGE(TAG, "Failed to start portal");
    }
}
