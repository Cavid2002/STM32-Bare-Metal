#include "../include/SPI.h"
#include "../include/RCC.h"
#include "../include/GPIO.h"


void SPI1_init()
{
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_SPI1;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_A;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_AFIO;

    GPIO_BASE_A->CFGR_LOW &= ~(0xFFFF << 16);
    GPIO_BASE_A->CFGR_LOW |= (0xB4B3 << 16);            
    GPIO_BASE_A->ODR |= (1 << 4); 
    SPI1_BASE->CR1 |= (1 << 2) | (1 << 9) | (1 << 8);
    SPI1_BASE->CR1 |= 1 << 6;
}

void CS_low()
{
    GPIO_BASE_A->ODR &= ~(1 << 4);
}

void CS_high()
{
    GPIO_BASE_A->ODR |= (1 << 4);
}


uint8_t SPI_transmit_poll(SPI_REGS* base, uint8_t data)
{
    while(!(base->SR & SPI_SR_TXE));

    base->DR = data;
    
    while(!(base->SR & SPI_SR_RXNE));

    return base->DR;
}


