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

typedef struct
{
    uint8_t dma_op;
    uint32_t lba;
    uint8_t* buff;
    uint32_t size; 
    volatile uint8_t done;
} DMA_Request;



#define DMA1_CHANNEL_COUNT 7
#define DMA2_CHANNEL_COUNT 5
#define CMD_SD_WRITE       24
#define CMD_SD_READ        17
#define DMA1_QUEUE_SIZE    8
#define DMA1_BASE ((DMA_REGS*)0x40020000)
#define DMA2_BASE ((DMA_REGS*)0x40020400)
#define DMA1_IRQ           12


extern DMA_Request dma_queue[DMA1_QUEUE_SIZE];
extern uint8_t dma_write_ptr;
extern uint8_t dma_read_ptr;


void DMA1_SD_transmit(uint8_t* tx, uint8_t* rx);
void DMA1_enable();
void DMA1_disable();
void DMA1_interrupt_handler();
void DMA1_complete_current();
uint8_t DMA1_start_next();
void DMA1_init();


#endif
