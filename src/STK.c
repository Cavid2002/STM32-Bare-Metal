#include "../include/STK.h"
#include "../include/RCC.h"
#include "../include/USART.h"

uint32_t count = 0;

void STK_enable(uint32_t time)
{

    STK_BASE->LOAD = time - 1;
    STK_BASE->VAL = 0;
    STK_BASE->CTRL |= 3 << 1;
    STK_BASE->CTRL |= 1 << 0;
}

uint32_t STK_status()
{
    return STK_BASE->CTRL >> 16;
}

uint32_t STK_val()
{
    return STK_BASE->VAL;
}



void STK_interrupt_handler()
{
    count++;
    USART1_write_line("STK interrupt\n\r");
}