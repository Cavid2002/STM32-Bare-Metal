#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>


typedef struct
{
    volatile uint32_t ISER[3];
    volatile uint32_t _reserved0[29];
    volatile uint32_t ICER[3];
    volatile uint32_t _reserved1[29];
    volatile uint32_t ISPR[3];
    volatile uint32_t _reserved2[29];
    volatile uint32_t ICPR[3];
    volatile uint32_t _reserved3[29];
    volatile uint32_t IABR[3];
    volatile uint32_t _reserved4[61];
    volatile uint32_t IPR[20];
} NVIC_REGS;


#define NVIC_BASE ((NVIC_REGS*)0xE000E100)
#define SWT_INTR  ((uint32_t*)0xE000EF00)

void NVIC_enable_irq(uint32_t irq_num);
void NVIC_disable_irq(uint32_t irq_num);

uint32_t NVIC_get_enable(uint32_t irq_num);
void NVIC_set_pending(uint32_t irq_num);
void NVIC_clear_pending(uint32_t irq_num);
uint32_t NVIC_get_pending(uint32_t irq_num);
uint32_t NVIC_get_active(uint32_t irq_num);
void NVIC_set_priority(uint32_t irq_num, uint8_t priority);
uint8_t NVIC_get_priority(uint32_t irq_num);


extern uint32_t vector_table[];

#endif