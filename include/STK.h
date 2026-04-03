#ifndef STK_H
#define STK_H

#include <stdint.h>


typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} STK_REGS;

#define STK_BASE ((STK_REGS*)0xE000E010)


void STK_enable(uint32_t time);

uint32_t STK_status();

uint32_t STK_val();

void STK_interrupt_handler();

#endif