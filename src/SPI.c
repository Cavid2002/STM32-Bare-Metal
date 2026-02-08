#include "../include/SPI.h"
#include "../include/RCC.h"

void SPI1_init()
{
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_SPI1;
    SPI1_BASE->CR1 |= 1 << 11 | 1 << 2;
    SPI1_BASE->CR1 |= 1 << 6;
}



void SPI_write_poll(SPI_REGS* base, uint16_t data)
{
    while(!(base->SR & SPI_SR_TXE));
    base->DR = data;
}

uint32_t SPI_read_poll(SPI_REGS* base)
{
    while(!(base->SR & SPI_SR_RXNE));
    return base->DR;
}

