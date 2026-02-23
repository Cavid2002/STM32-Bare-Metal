#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"


char start[SECTOR_SIZE] = "Hello World\r\n";
char temp[SECTOR_SIZE];



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
    RCC_HSI_enable();
    RCC_APB2DevEnable(RCC_APB2_ENB_PORT_A);
    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_A, 1, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    USART1_init(9600);
    SD_init();
    SD_write_block(start, 1);   
    SD_read_block(temp, 1); 

    while(1)
    {
        
        start[counter % 10] += 1;
        USART_write_line(USART1_BASE, start);
        USART_write_line(USART1_BASE, temp);

        key = USART_read_poll(USART1_BASE);
        USART_write_poll(USART1_BASE, key);
        GPIO_pinToggle(GPIO_BASE_A, 1);
        GPIO_pinToggle(GPIO_BASE_A, 0);
        counter++;
    }
}


