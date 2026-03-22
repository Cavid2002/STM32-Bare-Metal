#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define SCREEN_H    135
#define SCREEN_W    240

#define OFF_X    40
#define OFF_Y    52

#define LCD_CMD     0
#define LCD_DATA    1



void lcd_delay(uint32_t delay);


void lcd_cmd(uint8_t cmd);

void lcd_data(uint8_t data);

void lcd_init();

void lcd_move_cursor(uint16_t x0, uint16_t y0, 
    uint16_t x1, uint16_t y1);

void lcd_put_pixel(uint16_t pixel);

void lcd_clear_screen(uint16_t color);

#endif