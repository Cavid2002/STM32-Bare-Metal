#include "../include/NVIC.h"
#include <stdint.h>


void NVIC_enable_irq(uint32_t irq_num)
{
    NVIC_BASE->ISER[irq_num / 32] = 1 << (irq_num % 32);
}

void NVIC_disable_irq(uint32_t irq_num)
{
    NVIC_BASE->ICER[irq_num / 32] = 1 << (irq_num % 32);
}

uint32_t NVIC_get_enable(uint32_t irq_num)
{
    return NVIC_BASE->ICER[irq_num / 32] & (1 << (irq_num % 32));
}

void NVIC_set_pending(uint32_t irq_num)
{
    NVIC_BASE->ISPR[irq_num / 32] |= 1 << (irq_num % 32);
}

void NVIC_clear_pending(uint32_t irq_num)
{
    NVIC_BASE->ICPR[irq_num / 32] |= 1 << (irq_num % 32);
}

uint32_t NVIC_get_pending(uint32_t irq_num)
{
    return NVIC_BASE->ICPR[irq_num / 32] & 1 << (irq_num % 32);
}

uint32_t NVIC_get_active(uint32_t irq_num)
{
    return NVIC_BASE->IABR[irq_num / 32] & 1 << (irq_num % 32);
}

void NVIC_set_priority(uint32_t irq_num, uint8_t priority)
{
    uint32_t index = irq_num / 4;
    uint32_t offset = irq_num % 4;
    NVIC_BASE->IPR[index] &= ~(0xFF << (offset * 8));
    NVIC_BASE->IPR[index] |= priority << (offset * 8);
}

uint8_t NVIC_get_priority(uint32_t irq_num)
{
    uint32_t index = irq_num / 4;
    uint32_t offset = irq_num % 4;
    return (NVIC_BASE->IPR[index] >> (offset * 8)) & 0xFF;
}

void NVIC_asign_isr(uint32_t index, uint32_t isr_addr)
{
    vector_table[index] = isr_addr;
}