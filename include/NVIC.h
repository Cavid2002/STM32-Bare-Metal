#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>


typedef struct
{
    volatile uint32_t ISER[3];
    volatile uint32_t ICER[3];
    volatile uint32_t ISPR[3];
    volatile uint32_t ICPR[3];
    volatile uint32_t IABR[3];
    volatile uint32_t IPR[20];
} NVIC_REGS;


#define NVIC_BASE ((NVIC_REGS*)0xE000E100)
#define SWT_INTR  ((uint32_t*)0xE000E100 + 0xE00)

extern uint32_t vector_table[];

#endif