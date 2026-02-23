#include "../include/SPI.h"
#include "../include/RCC.h"
#include "../include/GPIO.h"


void SPI1_init()
{
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_AFIO;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_SPI1;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_A;
    
    GPIO_BASE_A->CFGR_LOW &= ~(0xFFFF << 16);
    GPIO_BASE_A->CFGR_LOW |= 0xB4BB << 16;

    SPI1_BASE->CR1 |= 1 << 9 | 1 << 8 | 1 << 2;
    SPI1_BASE->CR1 |= 1 << 6;
}

void CS_low(SPI_REGS* base)
{
    base->CR1 &= ~(1 << 8);
}

void CS_high(SPI_REGS* base)
{
    base->CR1 |= 1 << 8;
}


uint8_t SPI_transmit_poll(SPI_REGS* base, uint8_t data)
{
    while(!(base->SR & SPI_SR_TXE));

    base->DR = data;
    
    while(!(base->SR & SPI_SR_RXNE))

    return base->DR;
}

uint8_t SPI_read_poll(SPI_REGS* base)
{
    return SPI_transmit_poll(base, 0xFF);
}

void SPI_write_poll(SPI_REGS* base, uint8_t data)
{
    SPI_transmit_poll(base, data);
    return;
}


