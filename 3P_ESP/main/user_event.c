#include "user_event.h"
#include "lcd.h"
#include "buzzer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "esp_timer.h"

static const char *TAG = "USER_EVENT";
extern const char *AP_SSID;

static esp_timer_handle_t reset_timer = NULL;
static void lcd_show_network_ready(void);

static void reset_lcd_cb(void *arg) {
    lcd_show_network_ready();
}

static void lcd_show_network_ready(void)
{
    lcd_clear();
    lcd_print_at(0, 0, "Network Ready");
    lcd_scroll_text(1, AP_SSID, 250);
}


void on_login_success(const char *username)
{
    lcd_clear();
    lcd_print_at(0, 0, "User:");
    lcd_scroll_text(0, username, 250);
    lcd_print_at(0, 1, "Friend added!");
    buzzer_beep_confirm();

    if (!reset_timer) {
        const esp_timer_create_args_t args = {
            .callback = reset_lcd_cb,
            .name = "lcd_reset_timer"
        };
        esp_timer_create(&args, &reset_timer);
    }
    esp_timer_stop(reset_timer);
    esp_timer_start_once(reset_timer, 5000000);
}


void lcd_animate_starting(int times, int delay_ms)
{
    for (int i = 0; i < times; i++) {
        for (int dot = 1; dot <= 3; dot++) {
            lcd_print_at(0, 1, "Starting"); 
            for (int d = 0; d < dot; d++) {
                lcd_print_at(8 + d, 1, ".");
            }
            for (int c = dot; c < 3; c++) {
                lcd_print_at(8 + c, 1, " ");
            }
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }
    }
}
