#ifndef USART_H
#define USART_H

#include <stdint.h>

#define USART_STATUS_CTS     9
#define USART_STATUS_LBD     8
#define USART_STATUS_TXE     7
#define USART_STATUS_TC      6      
#define USART_STATUS_RXNE    5
#define USART_STATUS_IDLE    4
#define USART_STATUS_ORE     3
#define USART_STATUS_NE      2
#define USART_STATUS_FE      1
#define USART_STATUS_PE      0

#define USART_CR1_UE         13
#define USART_CR1_M          12
#define USART_CR1_WAKE       11
#define USART_CR1_PCE        10
#define USART_CR1_PS         9
#define USART_CR1_PEIE       8
#define USART_CR1_TXEIE      7
#define USART_CR1_TCIE       6
#define USART_CR1_RXNEIE     5
#define USART_CR1_UDLEIE     4
#define USART_CR1_TE         3
#define USART_CR1_RE         1
#define USART_CR1_SBK        0

#define USART_CR2_LINEN      14
#define USART_CR2_STOP       12
#define USART_CR2_CLKEN      11
#define USART_CR2_CPOL       10
#define USART_CR2_CPHA       9
#define USART_CR2_LBCL       8
#define USART_CR2_LINBDIE    6
#define USART_CR2_LBDL       5
#define USART_CR2_ADD        0

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_REGS; 


#define USART1_BASE ((USART_REGS*)0x40013800)
#define USART2_BASE ((USART_REGS*)0x40004400)
#define USART3_BASE ((USART_REGS*)0x40004800) 
#define USART4_BASE ((USART_REGS*)0x40004C00) 
#define USART5_BASE ((USART_REGS*)0x40005000)


#endif
