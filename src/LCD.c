#include "../include/LCD.h"
#include "../include/SPI.h"

void lcd_delay(uint32_t delay)
{
    for(uint32_t i = 0; i < delay; i++);
}

void lcd_send(uint8_t data, uint8_t type)
{
    
    if(type == LCD_DATA) DC_low();
    else if(type == LCD_DATA) DC_high();
    CS2_low();

    SPI2_send_poll(cmd);
    CS2_high();
}

void lcd_init()
{
    lcd_send(0x01, LCD_CMD);
    lcd_delay(10000);
    
    lcd_send(0x11, LCD_CMD);
    lcd_delay(10000);

    lcd_send(0x3A, LCD_CMD);
    lcd_send(0x55, LCD_DATA);

    lcd_send(0x36, LCD_CMD);
    lcd_data(0x00, LCD_DATA);
    

    lcd_send(0x2A, LCD_CMD);
    lcd_send()
}
