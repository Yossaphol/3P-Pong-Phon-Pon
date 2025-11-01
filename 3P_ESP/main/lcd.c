#include "lcd.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>


#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_PORT I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 50000
#define LCD_ADDR 0x27


#define RS            0x01
#define RW            0x02
#define EN            0x04
#define LCD_BACKLIGHT 0x08

static const char *TAG = "LCDDRV";

static esp_err_t i2c_write_byte(uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LCD_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t lcd_pulse(uint8_t data) {
    esp_err_t r;
    r = i2c_write_byte(data | EN | LCD_BACKLIGHT);
    if (r != ESP_OK) return r;
    ets_delay_us(500);
    r = i2c_write_byte((data & ~EN) | LCD_BACKLIGHT);
    ets_delay_us(500);
    return r;
}


static esp_err_t lcd_send_nibble(uint8_t nibble, bool is_data) {
    uint8_t data = (nibble & 0xF0);
    if (is_data) data |= RS;        
    data |= LCD_BACKLIGHT;        
    return lcd_pulse(data);
}


static esp_err_t lcd_send_byte_full(uint8_t value, bool is_data) {
    esp_err_t r;
    r = lcd_send_nibble(value & 0xF0, is_data);
    if (r != ESP_OK) return r;
    r = lcd_send_nibble((value << 4) & 0xF0, is_data);
    return r;
}

static inline esp_err_t lcd_cmd(uint8_t cmd)  { return lcd_send_byte_full(cmd, false); }
static inline esp_err_t lcd_data(uint8_t data){ return lcd_send_byte_full(data, true); }

static esp_err_t lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40};
    if (row > 1) row = 1;
    return lcd_cmd(0x80 | (col + row_offsets[row]));
}

static void lcd_raw_nibble(uint8_t nib) {
    lcd_pulse(nib | LCD_BACKLIGHT);
}

void lcd_scroll_text(uint8_t row, const char *text, int delay_ms) {
    const int LCD_WIDTH = 16;
    int len = strlen(text);

    if (len <= LCD_WIDTH) {
        lcd_print_at(0, row, text);
        return;
    }

    char window[LCD_WIDTH + 1];
    for (int i = 0; i <= len - LCD_WIDTH; i++) {
        memcpy(window, &text[i], LCD_WIDTH);
        window[LCD_WIDTH] = '\0';
        lcd_print_at(0, row, window);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    for (int j = 0; j < LCD_WIDTH; j++) {
        memcpy(window, &text[j], LCD_WIDTH);
        window[LCD_WIDTH] = '\0';
        lcd_print_at(0, row, window);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}


esp_err_t lcd_init(void) {
    static bool i2c_inited = false;
    if (!i2c_inited) {
        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = I2C_MASTER_SDA_IO,
            .scl_io_num = I2C_MASTER_SCL_IO,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = I2C_MASTER_FREQ_HZ
        };
        ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_PORT, &conf));
        ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_PORT, conf.mode, 0, 0, 0));
        i2c_inited = true;
    }

    vTaskDelay(pdMS_TO_TICKS(50));

    lcd_raw_nibble(0x30);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_raw_nibble(0x30);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_raw_nibble(0x30);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_raw_nibble(0x20);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_cmd(0x28); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_cmd(0x08); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_cmd(0x01); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_cmd(0x06); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_cmd(0x0C); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_clear();   vTaskDelay(pdMS_TO_TICKS(5));

    ESP_LOGI(TAG, "LCD init done");
    return ESP_OK;
}

esp_err_t lcd_clear(void) {
    esp_err_t r = lcd_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(5));
    return r;
}

esp_err_t lcd_print_at(uint8_t col, uint8_t row, const char *text) {
    esp_err_t r = lcd_set_cursor(col, row);
    if (r != ESP_OK) return r;
    while (*text) {
        r = lcd_data((uint8_t)*text++);
        if (r != ESP_OK) return r;
    }
    return ESP_OK;
}
