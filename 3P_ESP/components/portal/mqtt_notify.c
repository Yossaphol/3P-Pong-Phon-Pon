#include "mqtt_notify.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "portal.h"
#include "lwip/lwip_napt.h"
#include "cJSON.h"
#include <string.h>
#include <stdio.h>
#include "lcd.h"
#include "buzzer.h"
#include "user_event.h"


static const char *TAG = "MQTT_NOTIFY";
static esp_mqtt_client_handle_t mqtt_client = NULL;

static void enable_nat_now(void)
{
    esp_netif_t *ap = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (!ap) {
        ESP_LOGW(TAG, "AP netif not found for NAT");
        return;
    }

    esp_netif_ip_info_t ip;
    if (esp_netif_get_ip_info(ap, &ip) == ESP_OK) {
        ip_napt_enable(ip.ip.addr, 1);
        ESP_LOGI(TAG, "NAT enabled by MQTT trigger");
    } else {
        ESP_LOGW(TAG, "Failed to get AP IP info");
    }
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED: {
            ESP_LOGI(TAG, "Connected to HiveMQ broker");
            const char *topic = "offlearn/notify_login";
            int mid = esp_mqtt_client_subscribe(mqtt_client, topic, 1);
            ESP_LOGI(TAG, "Subscribed to topic: %s (mid=%d)", topic, mid);
            break;
        }

        case MQTT_EVENT_DATA: {
            char msg[512] = {0};
            int len = event->data_len < sizeof(msg) - 1 ? event->data_len : sizeof(msg) - 1;
            memcpy(msg, event->data, len);
            msg[len] = '\0';

            ESP_LOGI(TAG, "MQTT message: %s", msg);

            cJSON *root = cJSON_Parse(msg);
            if (!root) {
                ESP_LOGW(TAG, "Invalid JSON format");
                break;
            }

            const cJSON *userId = cJSON_GetObjectItem(root, "userId");
            const cJSON *name = cJSON_GetObjectItem(root, "name");

            if (cJSON_IsString(userId) && cJSON_IsString(name)) {
                ESP_LOGI(TAG, "UserID: %s", userId->valuestring);
                ESP_LOGI(TAG, "Name: %s", name->valuestring);

                on_login_success(name->valuestring);

                enable_nat_now();
                nat_enable_temporarily(180);
                portal_set_logged_in(NULL, true);
                ESP_LOGI(TAG, "Portal marked as logged in");
            } else {
                ESP_LOGW(TAG, "Missing userId or name field");
            }

            cJSON_Delete(root);
            break;
        }

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "Disconnected from HiveMQ broker");
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR occurred");
            break;

        default:
            break;
    }
    return ESP_OK;
}

static void start_mqtt_after_ip(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "STA got IP, starting MQTT...");
        if (mqtt_client) {
            esp_mqtt_client_start(mqtt_client);
        }
    }
}

void mqtt_notify_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://broker.hivemq.com",
        .port = 1883,
        .client_id = "offlearn_esp32",
        .keepalive = 60,
        .disable_auto_reconnect = false,
        .event_handle = mqtt_event_handler,
    };

    ESP_LOGI(TAG, "MQTT init to %s:%d", mqtt_cfg.uri, mqtt_cfg.port);

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &start_mqtt_after_ip, NULL);

    esp_netif_t *sta = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    esp_netif_ip_info_t ip;
    if (sta && esp_netif_get_ip_info(sta, &ip) == ESP_OK && ip.ip.addr != 0) {
        ESP_LOGI(TAG, "STA already has IP, starting MQTT immediately");
        esp_mqtt_client_start(mqtt_client);
    }

    ESP_LOGI(TAG, "MQTT client initialized (waiting for STA IP...)");
}
