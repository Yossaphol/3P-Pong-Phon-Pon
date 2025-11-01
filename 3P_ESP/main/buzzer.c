#include "buzzer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUZZER_GPIO 25
#define BUZZER_ON  0
#define BUZZER_OFF 1

void buzzer_init(void) {
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(BUZZER_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_level(BUZZER_GPIO, BUZZER_OFF);
}

void buzzer_beep_confirm(void) {
    gpio_set_level(BUZZER_GPIO, BUZZER_ON);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(BUZZER_GPIO, BUZZER_OFF);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(BUZZER_GPIO, BUZZER_ON);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(BUZZER_GPIO, BUZZER_OFF);
}

void buzzer_beep_error(void) {
    gpio_set_level(BUZZER_GPIO, BUZZER_ON);
    vTaskDelay(pdMS_TO_TICKS(400));
    gpio_set_level(BUZZER_GPIO, BUZZER_OFF);
}
