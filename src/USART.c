#include "../include/USART.h"
#include "../include/RCC.h"
#include "../include/AFIO.h"



void USART_config(USART_REGS* base, 
    uint32_t baud_rate, uint32_t word_len, uint32_t parity_type)
{
    
    if(base == USART1_BASE) AFIO_BASE->MAPR |= 1 << 2; 
    
    base->CR1 |= 1 << USART_CR1_UE | 1 << USART_CR1_TE 
                    | 1 << word_len | 1 << parity_type;
    
    base->BRR = baud_rate;
} 

uint32_t USART_compute_baud(uint32_t apb_clk, uint32_t baud_rate)
{
    
}


int USART_poll(USART_REGS* base, uint8_t flag, uint16_t timeout)
{
    while(timeout)
    {
        if(base->SR & flag) return flag;
        timeout--;
    }

    return -1;
}

void USART_write_poll(USART_REGS* base, uint8_t c)
{
    while(!(base->SR & 1 << USART_STATUS_TXE));
    base->DR = c;
}

uint8_t USART_read_poll(USART_REGS* base)
{
    while(!(base->SR & 1 << USART_STATUS_RXNE))
    return base->DR;
}

void USART_interrupt_enable(USART_REGS* base, uint32_t iflag)
{
    base->CR1 |= base;
}