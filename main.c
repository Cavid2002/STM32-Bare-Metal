#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"
#include "./include/LCD.h"


char start[SECTOR_SIZE] = "Hello from SD card\r\n";
char temp[SECTOR_SIZE];
char temp2[SECTOR_SIZE];



void delay(uint32_t delay)
{
    while(delay)
    {
        delay--;
    }
}



int main()
{
    uint8_t key = 0, counter = 0;
    RCC_HSE_PLL_enable();
    RCC_APB2DevEnable(RCC_APB2_ENB_PORT_A);
    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_A, 1, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    
    USART1_init(115200);
    SPI1_init();
    SPI2_init();

    SD_begin();
    SD_read_block(temp, 200);
       
    LCD_init();
    LCD_clear_screen(0xFFFF);

    USART_write_line(USART1_BASE, start);

    while(1)
    {
        LCD_clear_screen(0xFFFF);
        LCD_clear_screen(0xFFF0);
        LCD_clear_screen(0xFF00);
        LCD_clear_screen(0xF000);
        LCD_clear_screen(0x0000);
    }
}


