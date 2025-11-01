// NVS console commands registration
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_console.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#include "router_globals.h"  // for get_config_param_* helpers

static const char *TAG = "cmd_nvs";

static int cmd_nvs_erase(int argc, char **argv)
{
    (void)argc; (void)argv;
    esp_err_t err = nvs_flash_erase();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_flash_erase failed: %s", esp_err_to_name(err));
        return err;
    }
    err = nvs_flash_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_flash_init failed: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "NVS erased");
    return 0;
}

static void print_str_param(const char *key)
{
    char *val = NULL;
    esp_err_t err = get_config_param_str((char *)key, &val);
    if (err == ESP_OK && val) {
        printf("%s: %s\n", key, val);
        free(val);
    } else {
        printf("%s: <not set>\n", key);
    }
}

static int cmd_nvs_show(int argc, char **argv)
{
    (void)argc; (void)argv;
    puts("NVS parameters:");
    print_str_param("ssid");
    print_str_param("ent_username");
    print_str_param("ent_identity");
    print_str_param("passwd");
    print_str_param("static_ip");
    print_str_param("subnet_mask");
    print_str_param("gateway_addr");
    print_str_param("ap_ssid");
    print_str_param("ap_passwd");
    print_str_param("ap_ip");
    print_str_param("lock");
    return 0;
}

void register_nvs(void)
{
    const esp_console_cmd_t erase_cmd = {
        .command = "nvs_erase",
        .help = "Erase NVS partition",
        .hint = NULL,
        .func = &cmd_nvs_erase,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&erase_cmd));

    const esp_console_cmd_t show_cmd = {
        .command = "nvs_show",
        .help = "Show saved NVS parameters",
        .hint = NULL,
        .func = &cmd_nvs_show,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&show_cmd));
}

