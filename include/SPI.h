#ifndef SPI_H
#define SPI_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCSPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
} SPI_REGS;

#define SPI1_BASE ((SPI_REGS*)(0x40013000))

#endif