#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"
#include "./include/LCD.h"
#include "./include/DMA.h"
#include "./include/STK.h"
#include <string.h>


char start[30];
char temp[30];

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
    
    STK_enable(100000);


    // strncpy(start, "Testing2\r\n", 30);
    // for(int i = 0; i < 10; i++)
    // {
    //     SD_write(start, 1000 + i, 30);
    // }

    // memset(temp, 0, 30);

    // for(int i = 0; i < 10; i++)
    // {
    //     SD_read(temp, 1000 + i, 30);
    //     USART1_write_line(temp);
    // }

    
    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 0);
        delay(1000000);
    }
}


