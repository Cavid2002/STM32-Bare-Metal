#include "../include/RCC.h"


void rcc_set()
{  
    RCC_BASE_ADDR->CR |= 1 << 0;
    RCC_BASE_ADDR->CFGR &= ~(3);
    RCC_BASE_ADDR->APB2_ENBR |= 1 << 4 | 1 << 3 | 1 << 2;
}



void RCC_APB1DevEnable(uint32_t dev)
{
    RCC_BASE_ADDR->APB1_ENBR |= 1 << dev;
}
void RCC_APB2DevEnable(uint32_t dev)
{
    RCC_BASE_ADDR->APB2_ENBR |= 1 << dev;
}
void RCC_AHBDevEnable(uint32_t dev)
{
    RCC_BASE_ADDR->AHB_ENBR |= 1 << dev;
}


void RCC_APB1DevDisable(uint32_t dev)
{
    RCC_BASE_ADDR->APB1_ENBR &= ~(1 << dev);
}
void RCC_APB2DevDisable(uint32_t dev)
{
    RCC_BASE_ADDR->APB2_ENBR &= ~(1 << dev);
}
void RCC_AHBDevDisable(uint32_t dev)
{
    RCC_BASE_ADDR->AHB_ENBR &= ~(1 << dev);
}