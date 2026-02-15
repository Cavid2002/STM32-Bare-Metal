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

void SPI_start(SPI_REGS* base)
{
    base->CR1 &= ~(1 << 8);
}

void SPI_stop(SPI_REGS* base)
{
    base->CR1 |= 1 << 8;
}


uint8_t SPI_transmit(SPI_REGS* base, uint8_t data)
{
    SPI_start(base);

    while(!(base->SR & SPI_SR_TXE));

    base->DR = base;
    
    while(!(base->SR & SPI_SR_RXNE))

    SPI_stop(base);
    return base->DR;
}

uint8_t SPI_read_poll(SPI_REGS* base)
{
    return SPI_transmit(0x00);
}

void SPI_write_poll(SPI_REGS* base, uint8_t data)
{
    SPI_transmit(data);
    return;
}

