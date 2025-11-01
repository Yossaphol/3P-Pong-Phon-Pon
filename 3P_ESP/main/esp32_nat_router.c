#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"
#include "lwip/sockets.h"
#include "lwip/lwip_napt.h"
#include "lwip/ip4_addr.h"
#include "lwip/inet.h"

#include "lcd.h"
#include "buzzer.h"
#include "user_event.h"

#include "dhcpserver/dhcpserver.h"
#include "dhcpserver/dhcpserver_options.h"

#include "portal.h"
extern void dns_hijack_start(ip4_addr_t ap_ip);

#define TAG "ESP32_NAT"

#define STA_SSID        "NT wifi 3204-2.4G"
#define STA_PASSWORD    "0825508806"

#define AP_IP_STR       "192.168.4.1"
#define AP_MASK_STR     "255.255.255.0"

const char *AP_SSID     = "PongPhonPon Wifi";
const char *AP_PASSWORD = "12345678";

esp_netif_t *wifiAP  = NULL;
esp_netif_t *wifiSTA = NULL;

static bool s_portal_started = false;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {

        esp_wifi_connect();

    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {

        ESP_LOGW(TAG, "STA disconnected, reconnecting...");
        esp_wifi_connect();

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {

        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "STA got IP: " IPSTR ", mask: " IPSTR ", gw: " IPSTR,
                 IP2STR(&event->ip_info.ip),
                 IP2STR(&event->ip_info.netmask),
                 IP2STR(&event->ip_info.gw));

        esp_netif_ip_info_t ap_ip;
        if (esp_netif_get_ip_info(wifiAP, &ap_ip) == ESP_OK) {
            ip_napt_enable(ap_ip.ip.addr, 1);
            ESP_LOGI(TAG, "NAT enabled on AP " IPSTR, IP2STR(&ap_ip.ip));

            lcd_clear();
            lcd_print_at(0, 0, "Network Ready");
            lcd_scroll_text(1, AP_SSID, 250);
            buzzer_beep_confirm();


            if (!s_portal_started) {
                portal_init(wifiAP);
                dns_hijack_start(*(ip4_addr_t *)&ap_ip.ip);
                s_portal_started = true;
                ESP_LOGI(TAG, "Portal started (HTTP + DNS hijack)");
            }
        }

        esp_netif_dns_info_t local_dns;
        local_dns.ip.type = ESP_IPADDR_TYPE_V4;
        local_dns.ip.u_addr.ip4.addr = ipaddr_addr(AP_IP_STR);
        esp_netif_set_dns_info(wifiAP, ESP_NETIF_DNS_MAIN, &local_dns);
        ESP_LOGI(TAG, "AP DNS forced to self: " IPSTR, IP2STR(&local_dns.ip.u_addr.ip4));
    }
}

static void wifi_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifiAP  = esp_netif_create_default_wifi_ap();
    wifiSTA = esp_netif_create_default_wifi_sta();

    esp_netif_ip_info_t ap_ip_info = {0};
    ip4addr_aton(AP_IP_STR,   (ip4_addr_t*)&ap_ip_info.ip);
    ip4addr_aton(AP_IP_STR,   (ip4_addr_t*)&ap_ip_info.gw);
    ip4addr_aton(AP_MASK_STR, (ip4_addr_t*)&ap_ip_info.netmask);
    esp_netif_dhcps_stop(wifiAP);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(wifiAP, &ap_ip_info));
    esp_netif_dhcps_start(wifiAP);

    dhcps_offer_t dhcps_dns_value = OFFER_DNS;
    esp_netif_dhcps_option(wifiAP,
                           ESP_NETIF_OP_SET,
                           ESP_NETIF_DOMAIN_NAME_SERVER,
                           &dhcps_dns_value,
                           sizeof(dhcps_dns_value));

    esp_netif_dns_info_t dns_ap;
    dns_ap.ip.type = ESP_IPADDR_TYPE_V4;
    dns_ap.ip.u_addr.ip4.addr = ipaddr_addr("8.8.8.8");
    esp_netif_set_dns_info(wifiAP, ESP_NETIF_DNS_MAIN, &dns_ap);

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t ap_config = { 0 };
    strncpy((char *)ap_config.ap.ssid, AP_SSID, sizeof(ap_config.ap.ssid));
    ap_config.ap.ssid_len       = 0;
    ap_config.ap.max_connection = 8;
    ap_config.ap.channel        = 1;
    if (strlen(AP_PASSWORD) >= 8) {
        strncpy((char *)ap_config.ap.password, AP_PASSWORD, sizeof(ap_config.ap.password));
        ap_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    } else {
        ap_config.ap.password[0] = '\0';
        ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    wifi_config_t sta_config = { 0 };
    strncpy((char *)sta_config.sta.ssid,     STA_SSID,     sizeof(sta_config.sta.ssid));
    strncpy((char *)sta_config.sta.password, STA_PASSWORD, sizeof(sta_config.sta.password));
    sta_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,  &ap_config));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Wi-Fi init done (AP: %s / STA: %s)", AP_SSID, STA_SSID);

    ESP_LOGI(TAG, "Init LCD & Buzzer...");
    vTaskDelay(pdMS_TO_TICKS(500));
    lcd_init();
    buzzer_init();
    lcd_clear();
    lcd_print_at(0, 0, "OffLearn Router");
    lcd_print_at(0, 1, "Starting");
    lcd_animate_starting(10, 300);
    buzzer_beep_confirm();
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
}
