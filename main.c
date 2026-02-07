#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"





void delay(uint32_t delay)
{
    while(delay)
    {
        delay--;
    }
}



int main()
{
    uint8_t key = 0;
    RCC_HSE_PLL_enable();
    RCC_APB2DevEnable(RCC_APB2_ENB_PORT_A);
    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_A, 1, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    USART1_init(9600);

    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 0);
        GPIO_pinToggle(GPIO_BASE_A, 1);

        key = USART_read_poll(USART1_BASE);
        USART_write_poll(USART1_BASE, key);
       
    }
}


