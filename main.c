#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"
#include "./include/LCD.h"
#include "./include/DMA.h"


char start[30];

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
    DMA_init();

    if(SD_dma_read(start, 200, 30))
    {
        USART1_write_line("Request Enqueued\r\n");
    }    
    while(1)
    {
        SD_dma_read(start, 200, 30);
        USART1_write_line(start);
        GPIO_pinToggle(GPIO_BASE_A, 0);
        delay(1000000);
    }
}


