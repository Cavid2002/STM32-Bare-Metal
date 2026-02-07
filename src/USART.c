#include "../include/USART.h"
#include "../include/RCC.h"
#include "../include/AFIO.h"
#include "../include/GPIO.h"


void USART_write_poll(USART_REGS* base, uint8_t c)
{
    while(!(base->SR & (1 << USART_STATUS_TC)));
    base->DR = c;
}

uint8_t USART_read_poll(USART_REGS* base)
{
    while(!(base->SR & (1 << USART_STATUS_RXNE)));
    return base->DR;
}


int USART_write_line(USART_REGS* base, const char* str)
{
    int i = 0;
    while(str[i])
    {
        USART_write_poll(base, str[i]);
        i++;
    }
}

void USART1_init(uint32_t baud_rate)
{
    uint32_t apb_clk = 36000000;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_USART1;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_B;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_AFIO;
    AFIO_BASE->MAPR |= 1 << 2;

    USART1_BASE->BRR = ((apb_clk + (baud_rate/2U)) / baud_rate);  

    GPIO_BASE_B->CFGR_LOW &= ~(0xFF << 24);
    GPIO_BASE_B->CFGR_LOW |= 0b1011 << 24;
    GPIO_BASE_B->CFGR_LOW |= 0b0100 << 28;

    USART1_BASE->CR1 |= 1 << USART_CR1_UE | 1 << USART_CR1_TE | 1 << USART_CR1_RE;
    
}