#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"





void delay(uint32_t delay)
{
    while(delay)
    {
        delay--;
    }
}

int main()
{
    rcc_set();
    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_A, 1, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_A, 2, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_C, 13, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);

    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 2);
        GPIO_pinToggle(GPIO_BASE_A, 1);
        GPIO_pinToggle(GPIO_BASE_A, 0);
        delay(1000000);
    }
}


