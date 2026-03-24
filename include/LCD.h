#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define SCREEN_H    135
#define SCREEN_W    240

#define OFF_X    40
#define OFF_Y    52




void LCD_delay(uint32_t delay);


void LCD_cmd(uint8_t cmd);

void LCD_data(uint8_t data);

void LCD_init();

void LCD_move_cursor(uint16_t x0, uint16_t y0, 
    uint16_t x1, uint16_t y1);

void LCD_put_pixel(uint16_t pixel);

void LCD_clear_screen(uint16_t color);

#endif