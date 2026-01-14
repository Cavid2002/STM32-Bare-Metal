#ifndef AFIO_H
#define AFIO_H

#include <stdint.h>

typedef struct 
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR1;
    volatile uint32_t EXTICR2;
    volatile uint32_t EXTICR3;
    volatile uint32_t EXTICR4;
    volatile uint32_t MAPR2;
} AFIO_REGS;


void AFIO_remap()
{
    
}



#endif