
#ifndef RESET_H
#define RESET_H

#include <stdint.h>
#include "../include/USART.h"
#include "../include/DMA.h"

extern uint32_t _sbss, _ebss;
extern uint32_t _sdata, _edata, _ldata;
extern uint32_t _estack;

extern int main();
extern void _reset();
extern void USART1_interrupt_handler();
extern void DMA1_interrupt_handler();

uint32_t vector_table[84] __attribute__((section(".vector"))) = 
{
	[0] = (uint32_t)&_estack,
	[1] = (uint32_t)_reset,
	[27] = (uint32_t)DMA1_interrupt_handler,
	[53] = (uint32_t)USART1_interrupt_handler,
};


#endif




