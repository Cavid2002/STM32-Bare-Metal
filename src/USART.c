#include "../include/USART.h"
#include "../include/RCC.h"
#include "../include/AFIO.h"
#include "../include/GPIO.h"
#include "../include/NVIC.h"
#include "../include/SpinLock.h"
#include <string.h>

volatile uint32_t USART_lock = 0;
volatile char char_buff[USART_BUFF_SIZE];
volatile uint16_t read_ptr = 0;
volatile uint16_t write_prt = 0;

void USART_write_poll(USART_REGS* base, uint8_t c)
{
    while(!(base->SR & USART_STATUS_TXE));
    base->DR = c;
}

uint8_t USART_read_poll(USART_REGS* base)
{
    while(!(base->SR & USART_STATUS_RXNE));
    return base->DR;
}


void USART1_put_char(uint8_t c)
{
    if(c == '\r')
    {
        char_buff[write_prt % USART_BUFF_SIZE] = '\n';
        write_prt++;    
    }
    
    char_buff[write_prt % USART_BUFF_SIZE] = c;
    write_prt++;
    
    if(c == '\b')
    {
        char_buff[write_prt % USART_BUFF_SIZE] = ' ';
        write_prt++;
        char_buff[write_prt % USART_BUFF_SIZE] = '\b';
        write_prt++;
    } 
}

void USART1_write_char(uint8_t c)
{
    char_buff[write_prt % USART_BUFF_SIZE] = c;
    write_prt++;    
}

uint8_t USART1_read_char()
{
    if(read_ptr == write_prt) return 0;
    uint8_t res = char_buff[read_ptr % USART_BUFF_SIZE];
    read_ptr++;
    return res;
}

uint16_t USART1_write(char* buff, uint16_t size)
{
    uint16_t i = 0;
    for(i = 0; i < size; i++)
    {
        if(USART_BUFF_SIZE <= write_prt - read_ptr) break;
        USART1_write_char(buff[i]);
    }
    USART1_BASE->CR1 |= (USART_CR1_TXEIE);
    return i;
}

uint16_t USART1_read(char* buff, uint16_t size)
{
    uint16_t i = 0;
    for(i = 0; i < size; i++)
    {
        if(write_prt - read_ptr <= 0) break;
        buff[i] = USART1_read_char();
    }
    return i;
}

int USART1_write_line(const char* str)
{
    uint16_t size = strlen(str);
    uint16_t sum = 0;
    while(1)
    {
        if(sum == size) break;
        sum += USART1_write((char*)(str + sum), size - sum);
    }
    return sum;
}

void USART1_init(uint32_t baud_rate)
{
    uint32_t apb_clk = 72000000;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_USART1;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_PORT_B;
    RCC_BASE_ADDR->APB2_ENBR |= RCC_APB2_ENB_AFIO;
    AFIO_BASE->MAPR |= 1 << 2;

    USART1_BASE->BRR = ((apb_clk + (baud_rate/2U)) / baud_rate);  

    GPIO_BASE_B->CFGR_LOW &= ~(0xFF << 24);
    GPIO_BASE_B->CFGR_LOW |= 0b1011 << 24;
    GPIO_BASE_B->CFGR_LOW |= 0b0100 << 28;

    USART1_BASE->CR1 |=USART_CR1_TE | USART_CR1_RE;
    USART1_BASE->CR1 |= USART_CR1_UE;
    
}


void USART1_interrupt_enable()
{
    USART1_BASE->CR1 |= USART_CR1_TXEIE;
    USART1_BASE->CR1 |= USART_CR1_RXNEIE;
    NVIC_set_priority(USART1_IVT_INDEX, 0x10);
    NVIC_enable_irq(USART1_IVT_INDEX);
}

void USART1_interrupt_handler()
{
    if(USART1_BASE->SR & USART_STATUS_RXNE)
    {
        USART1_put_char(USART1_BASE->DR);
        USART1_BASE->CR1 |= USART_CR1_TXEIE;
    }

    if(USART1_BASE->SR & USART_STATUS_TXE)
    {
        if(read_ptr != write_prt) USART1_BASE->DR = USART1_read_char();
        else USART1_BASE->CR1 &= ~(USART_CR1_TXEIE);
    }
}

void USART1_print_number(int32_t n)
{
    char buf[12];
    uint8_t i = 0;
    uint8_t neg = 0;

    if(n == 0)
    {
        USART1_write_line("0");
        return;
    }

    if(n < 0)
    {
        neg = 1;
        n = -n;
    }

    while(n > 0)
    {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    if(neg) buf[i++] = '-';

    for(int l = 0, r = i - 1; l < r; l++, r--)
    {
        char tmp = buf[l];
        buf[l] = buf[r];
        buf[r] = tmp;
    }

    buf[i] = '\0';
    USART1_write_line(buf);
}