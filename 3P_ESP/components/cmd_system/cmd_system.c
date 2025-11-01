// System console commands registration
#include <stdio.h>
#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"
#include "sdkconfig.h"

static const char *TAG = "cmd_system";

// 'restart' command
static int cmd_restart(int argc, char **argv)
{
    (void)argc; (void)argv;
    ESP_LOGI(TAG, "Restarting...");
    esp_restart();
    return 0;
}

// 'heap' command: show free heap
static int cmd_heap(int argc, char **argv)
{
    (void)argc; (void)argv;
    printf("Free heap: %u bytes\n", (unsigned)esp_get_free_heap_size());
    return 0;
}

void register_system(void)
{
    const esp_console_cmd_t restart_cmd = {
        .command = "restart",
        .help = "Restart the ESP",
        .hint = NULL,
        .func = &cmd_restart,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&restart_cmd));

    const esp_console_cmd_t heap_cmd = {
        .command = "heap",
        .help = "Show free heap size",
        .hint = NULL,
        .func = &cmd_heap,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&heap_cmd));
}

