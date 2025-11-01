#ifndef LCD_H
#define LCD_H

#include "esp_err.h"
#include "driver/i2c.h"

#define LCD_I2C_PORT      I2C_NUM_0
#define LCD_SDA_GPIO      25
#define LCD_SCL_GPIO      26
#define LCD_I2C_FREQ_HZ   100000
#define LCD_I2C_ADDR      0x27

esp_err_t lcd_init(void);
esp_err_t lcd_clear(void);
esp_err_t lcd_print_at(uint8_t col, uint8_t row, const char *text);
void lcd_scroll_text(uint8_t row, const char *text, int delay_ms);
void lcd_animate_starting(int times, int delay_ms);


#endif
