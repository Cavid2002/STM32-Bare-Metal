#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"
#include "./include/LCD.h"


char start[SECTOR_SIZE] = "Hello from SD card\n\r";



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
    GPIO_pinToggle(GPIO_BASE_A, 1);    
    
    USART1_init(115200);
    USART1_interrupt_enable();
    SPI1_init();

    SD_begin();
    SD_read_block(temp, 200);    
    USART1_write_line(temp);
    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 0);
        delay(1000000);
    }
}


