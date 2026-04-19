#include "../include/SPI.h"
#include "../include/RCC.h"
#include "../include/GPIO.h"


void SPI1_init()
{
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_SPI1;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_A;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_AFIO;

    GPIO_BASE_A->CFGR_LOW &= ~(0xFFFF << 16);
    GPIO_BASE_A->CFGR_LOW |= (0xB8B3 << 16);            
    GPIO_BASE_A->ODR |= (1 << 4); 
    GPIO_BASE_A->ODR |= (1 << 6); 
    
    SPI1_BASE->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM;
    SPI1_BASE->CR1 |= SPI_CR1_SPE;
}


void SPI2_init()
{
    RCC_BASE_ADDR->APB1_ENBR |= RCC_APB1_ENB_SPI2;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_B;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_A;

    GPIO_BASE_B->CFGR_HIGH &= ~(0xFFFF << 16);
    GPIO_BASE_B->CFGR_HIGH |= (0xB3B3 << 16);
    GPIO_BASE_A->CFGR_HIGH &= ~(0xF << 0);
    GPIO_BASE_A->CFGR_HIGH |= (3 << 0);

    GPIO_BASE_B->ODR |= (1 << 12);
    GPIO_BASE_B->ODR |= (1 << 14);
    GPIO_BASE_A->ODR |= (1 << 8);

    SPI2_BASE->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM;
    SPI2_BASE->CR1 |= (1 << 0) | (1 << 1);
    SPI2_BASE->CR1 |= SPI_CR1_SPE;
}


void RST_high()
{
    GPIO_BASE_A->ODR |= (1 << 8);
}

void RST_low()
{
    GPIO_BASE_A->ODR &= ~(1 << 8); 
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
    while(!(SPI2_BASE->SR & SPI_SR_TXE));
    SPI2_BASE->DR = data;
    while(SPI2_BASE->SR & SPI_SR_BSY);
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


