#include "../include/RCC.h"

void RCC_HSI_enable()
{  
    RCC_BASE_ADDR->CR |= (1 << 0);
    
    
    while(!(RCC_BASE_ADDR->CR & (1 << 1)));
    
    volatile uint32_t *FLASH_ACR = (volatile uint32_t *)0x40022000;
    *FLASH_ACR |= (1 << 0);  
      
    RCC_BASE_ADDR->CFGR |= 7 << 18;      
    RCC_BASE_ADDR->CFGR &= ~(1 << 16);
    RCC_BASE_ADDR->CFGR |= 4 << 8;    
    
    RCC_BASE_ADDR->CR |= 1 << 24;
    while (!(RCC_BASE_ADDR->CR & 1 << 25));
    
    RCC_BASE_ADDR->CFGR |= 2 << 0;
    while ((RCC_BASE_ADDR->CFGR & 3 << 2) != 2 << 2);
}

void RCC_HSE_PLL_enable()
{
    RCC_BASE_ADDR->CR |= 1 << 16;
    while (!(RCC_BASE_ADDR->CR & 1 << 17));  
    
    volatile uint32_t *FLASH_ACR = (volatile uint32_t *)0x40022000;
    *FLASH_ACR |= (1 << 0);  
    
    RCC_BASE_ADDR->CFGR = 0;  
    RCC_BASE_ADDR->CFGR |= 7 << 18;   
    RCC_BASE_ADDR->CFGR |= 1 << 17;   
    RCC_BASE_ADDR->CFGR |= 1 << 16;   
    RCC_BASE_ADDR->CFGR |= 4 << 8;    
    
    RCC_BASE_ADDR->CR |= 1 << 24;
    while (!(RCC_BASE_ADDR->CR & 1 << 25));
    
    RCC_BASE_ADDR->CFGR |= 2 << 0;
    while ((RCC_BASE_ADDR->CFGR & 3 << 2) != 2 << 2);
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