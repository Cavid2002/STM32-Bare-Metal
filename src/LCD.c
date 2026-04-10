#include "../include/LCD.h"
#include "../include/SPI.h"

void LCD_delay(uint32_t delay)
{
    for(uint32_t i = 0; i < delay; i++);
}

void LCD_cmd(uint8_t cmd)
{
    DC_low();
    CS2_low();
    SPI2_send_poll(cmd);
    CS2_high();
}

void LCD_data(uint8_t data)
{
    DC_high();
    CS2_low();
    SPI2_send_poll(data);
    CS2_high();
}

void LCD_screen_adjust()
{
    LCD_cmd(0x36);
    LCD_data(0x60);
}

void LCD_init()
{
    RST_low();
    LCD_delay(1000000);
    RST_high();
    
    LCD_cmd(0x01);
    LCD_delay(1000000);
    
    LCD_cmd(0x11);
    LCD_delay(1000000);

    LCD_cmd(0x21);

    LCD_cmd(0x3A);
    LCD_data(0x55);

    LCD_cmd(0x13);

    LCD_cmd(0x29);
    LCD_cmd(0x38);

    LCD_screen_adjust();
}


void LCD_set_window(uint16_t x0, uint16_t y0, 
    uint16_t x1, uint16_t y1)
{
    x0 += OFF_X; x1 += OFF_X;
    y0 += OFF_Y; y1 += OFF_Y;
    LCD_cmd(0x2A);
    LCD_data(x0 >> 8); LCD_data(x0 & 0xFF);
    LCD_data(x1 >> 8); LCD_data(x1 & 0xFF);
    
    LCD_cmd(0x2B);
    LCD_data(y0 >> 8); LCD_data(y0 & 0xFF);
    LCD_data(y1 >> 8); LCD_data(y1 & 0xFF);

    LCD_cmd(0x2C);
}


void LCD_put_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_set_window(x, y, x, y);
    LCD_data(color >> 8);
    LCD_data(color & 0xFF);
}

void LCD_clear_screen(uint16_t color)
{
    LCD_set_window(0, 0, SCREEN_W, SCREEN_H);
    for(int i = 0; i < SCREEN_H; i++)
    {
        for(int j = 0; j < SCREEN_W; j++)
        {
            LCD_data(color >> 8);
            LCD_data(color & 0xFF);
        }
    }
}


