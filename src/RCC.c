#include "../include/RCC.h"


void rcc_set()
{  
    RCC_BASE_ADDR->CR |= 1 << 0;
    RCC_BASE_ADDR->CFGR &= ~(3);
    RCC_BASE_ADDR->APB2_ENBR |= 1 << 4 | 1 << 3 | 1 << 2;
}

void RCC_SysClkSelect(uint32_t clock_type)
{
    switch (clock_type)
    {
    case RCC_SW_HSI:
        RCC_BASE_ADDR->CR |= 1 << RCC_CR_HSIEN;
        break;
    case RCC_SW_HSE:
        RCC_BASE_ADDR->CR |= 1 << RCC_CR_HSEEN;
        break;
    case RCC_SW_PLL:
        RCC_BASE_ADDR->CR |= 1 << RCC_CR_PLLEN;
    default:
        break;
    }
    
    RCC_BASE_ADDR->CFGR &= ~(3 << RCC_CFGR_SW);
    RCC_BASE_ADDR->CFGR |= clock_type << RCC_CFGR_SW; 
}


int RCC_HSE_PLL_enable()
{
    RCC_BASE_ADDR->CR |= 1 << RCC_CR_HSEEN | 1 << RCC_CR_PLLEN;
    RCC_BASE_ADDR->CFGR |= 7 << 18 | 1 << 16 | 5 << 8 | 2 << 0;
    if(RCC_BASE_ADDR->CR & 1 << 19) return 0;

    return -1;
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