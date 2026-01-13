
#ifndef RESET_H
#define RESET_H

#include <stdint.h>

extern uint32_t _sbss, _ebss;
extern uint32_t _sdata, _edata, _ldata;
extern uint32_t _estack;


extern int main();

#endif




