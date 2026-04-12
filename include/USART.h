#ifndef USART_H
#define USART_H

#include <stdint.h>

// Status Register bits
#define USART_STATUS_CTS     1 << 9
#define USART_STATUS_LBD     1 << 8
#define USART_STATUS_TXE     1 << 7
#define USART_STATUS_TC      1 << 6      
#define USART_STATUS_RXNE    1 << 5
#define USART_STATUS_IDLE    1 << 4
#define USART_STATUS_ORE     1 << 3
#define USART_STATUS_NE      1 << 2
#define USART_STATUS_FE      1 << 1
#define USART_STATUS_PE      1 << 0

// Control Register 1 bits
#define USART_CR1_UE         1 << 13
#define USART_CR1_M          1 << 12
#define USART_CR1_WAKE       1 << 11
#define USART_CR1_PCE        1 << 10
#define USART_CR1_PS         1 << 9
#define USART_CR1_PEIE       1 << 8
#define USART_CR1_TXEIE      1 << 7
#define USART_CR1_TCIE       1 << 6
#define USART_CR1_RXNEIE     1 << 5
#define USART_CR1_IDLEIE     1 << 4   
#define USART_CR1_TE         1 << 3
#define USART_CR1_RE         1 << 2   
#define USART_CR1_RWU        1 << 1   
#define USART_CR1_SBK        1 << 0


#define USART_CR2_LINEN      1 << 14
#define USART_CR2_STOP       1 << 12
#define USART_CR2_CLKEN      1 << 11
#define USART_CR2_CPOL       1 << 10
#define USART_CR2_CPHA       1 << 9
#define USART_CR2_LBCL       1 << 8
#define USART_CR2_LBDIE      1 << 6
#define USART_CR2_LBDL       1 << 5
#define USART_CR2_ADD        1 << 0

#define USART_TIMEOUT        1000
#define USART_BUFF_SIZE      128

#define USART1_IVT_INDEX     37

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


void USART1_init(uint32_t baud_rate);
int USART1_write_line(const char* str);
void USART1_interrupt_handler();
void USART1_interrupt_enable();
uint16_t USART1_write(char* buff, uint16_t size);
uint16_t USART1_read(char* buff, uint16_t size);
void USART1_print_number(int32_t n);
uint8_t USART1_read_char();
void USART1_write_char(uint8_t c);

#define USART1_BASE ((USART_REGS*)0x40013800)
#define USART2_BASE ((USART_REGS*)0x40004400)
#define USART3_BASE ((USART_REGS*)0x40004800) 

#endif