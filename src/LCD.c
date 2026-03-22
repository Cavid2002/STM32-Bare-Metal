#include "../include/LCD.h"
#include "../include/SPI.h"

void lcd_delay(uint32_t delay)
{
    for(uint32_t i = 0; i < delay; i++);
}

void lcd_cmd(uint8_t cmd)
{
    DC_low();
    CS2_low();
    SPI2_send_poll(cmd);
    CS2_high();
}

void lcd_data(uint8_t data)
{
    DC_low();
    CS2_low();
    SPI2_send_poll(data);
    CS2_high();
}

void lcd_init()
{
    lcd_cmd(0x01);
    lcd_delay(10000);
    
    lcd_cmd(0x11);
    lcd_delay(10000);

    lcd_cmd(0x3A);
    lcd_data(0x55);

    lcd_cmd(0x36);
    lcd_data(0x60);
    
    lcd_cmd(0x21);
    lcd_cmd(0x29);
}


void lcd_move_cursor(uint16_t x0, uint16_t y0, 
    uint16_t x1, uint16_t y1)
{
    x0 += OFF_X; x1 += OFF_X;
    y0 += OFF_Y; y1 += OFF_Y;
    lcd_cmd(0x2A);
    lcd_data(x0 >> 8); lcd_data(x0 & 0xFF);
    lcd_data(x1 >> 8); lcd_data(x1 & 0xFF);
    
    lcd_cmd(0x2B);
    lcd_data(y0 >> 8); lcd_data(y0 & 0xFF);
    lcd_data(y1 >> 8); lcd_data(y1 & 0xFF);

    lcd_cmd(0x2C);
}


void lcd_put_pixel(uint16_t pixel)
{
    lcd_data(pixel >> 8);
    lcd_data(pixel & 0xFF);
}

void lcd_clear_screen(uint16_t color)
{
    lcd_move_cursor(0, 0, SCREEN_W, SCREEN_H);
    for(int i = 0; i < SCREEN_H; i++)
    {
        for(int j = 0; j < SCREEN_W; j++)
        {
            lcd_put_pixel(color);
        }
    }
}


