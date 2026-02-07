#include "../include/SPI.h"
#include "../include/RCC.h"

void SPI1_init()
{
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_SPI1;
    SPI1_BASE->CR1 |= 1 << 11;
    SPI1_BASE->CR1 |= 
}