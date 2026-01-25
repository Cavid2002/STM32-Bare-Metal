#include "../include/USART.h"
#include "../include/RCC.h"



void USART_Enable(uint32_t pclk, uint32_t baudrate)
{
    RCC_APB2DevEnable(RCC_APB2_ENB_USART1);
    USART1_BASE->CR1 |= 1 << USART_CR1_UE;
    USART1_BASE->CR1 |= 0 << USART_CR1_M;
    USART1_BASE->CR1 |= 1 << USART_CR1_TE;
}

int USART_Wait(USART_REGS* base, uint8_t flag, uint16_t timeout)
{
    while(timeout)
    {
        if(base->SR & flag) return flag;
        timeout--;
    }

    return -1;
}

void USART_Write(USART_REGS* base, uint8_t c)
{
    if(USART_Wait(base, USART_STATUS_TC, 1000))
    base->DR = c;
}

uint8_t USART_Read(USART_REGS* base)
{
    USART_Wait(base, USART_STATUS_RXNE, 1000);
    return base->DR;
}


