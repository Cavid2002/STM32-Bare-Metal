#ifndef DMA_H
#define DMA_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t CCR;
    volatile uint32_t CNDTR;
    volatile uint32_t CPAR;
    volatile uint32_t CMAR;
    volatile uint32_t _reserved;
} DMA_CHANNEL;

typedef struct
{
    volatile uint32_t ISR;
    volatile uint32_t IFCR;
    DMA_CHANNEL CHNL_REGS[7]; 
} DMA_REGS;


#define DMA1_CHANNEL_COUNT 7
#define DMA2_CHANNEL_COUNT 5
#define DMA1_BASE ((DMA_REGS*)0x40020000)
#define DMA2_BASE ((DMA_REGS*)0x40020400)

#endif
