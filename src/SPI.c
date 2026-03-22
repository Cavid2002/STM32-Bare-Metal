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


void SPI2_init()
{
    RCC_BASE_ADDR->APB1_ENBR |= RCC_APB1_ENB_SPI2;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_B;

    GPIO_BASE_B->CFGR_HIGH &= ~(0xFFFF << 16);
    GPIO_BASE_B->CFGR_HIGH |= (0xB3B3 << 16);

    GPIO_BASE_B->ODR |= (1 << 12);

    SPI2_BASE->CR1 |= 1 << 15;
    SPI2_BASE->CR1 |= 1 << 14;
    SPI2_BASE->CR1 |= (1 << 2) | (1 << 9) | (1 << 8);
    SPI2_BASE->CR1 |= 1 << 6;
    
}

void SPI2_set_tx()
{
    SPI2_BASE->CR1 |= 1 << 14; 
    GPIO_BASE_B->CFGR_HIGH &= ~(0xF << 28);
    GPIO_BASE_B->CFGR_HIGH |= (0xB << 28);
}

void SPI2_set_rx()
{
    GPIO_BASE_B->CFGR_HIGH &= ~(0xF << 28);
    GPIO_BASE_B->CFGR_HIGH |= (0x4 << 28);
    SPI2_BASE->CR1 &= ~(1 << 14); 
}


void DC_high()
{
    GPIO_BASE_B->ODR |= 1 << 14;
}

void DC_low()
{
    GPIO_BASE_B->ODR &= ~(1 << 14);
}

void SPI2_send_poll(uint8_t data)
{
    SPI2_set_tx();

    SPI2_BASE->DR = data;
    while(!(SPI2_BASE->SR & SPI_SR_TXE)); 
}

uint8_t SPI2_recieve_poll()
{
    SPI2_set_rx();
    while(!(SPI2_BASE->SR & SPI_SR_RXNE));
    SPI2_set_tx();
    return SPI2_BASE->DR;
}

void CS2_low()
{
    GPIO_BASE_B->ODR &= ~(1 << 12);
}

void CS2_high()
{
    GPIO_BASE_B->ODR |= (1 << 12);
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


