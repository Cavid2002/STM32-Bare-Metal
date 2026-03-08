
#ifndef RESET_H
#define RESET_H

#include <stdint.h>

extern uint32_t _sbss, _ebss;
extern uint32_t _sdata, _edata, _ldata;
extern uint32_t _estack;

extern int main();
void _reset();

uint32_t vector_table[60] __attribute__((section(".vector"))) = 
{
	(uint32_t)&_estack,
	(uint32_t)_reset

};


#endif




