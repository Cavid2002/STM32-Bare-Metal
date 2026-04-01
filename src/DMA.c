#include "../include/DMA.h"
#include "../include/RCC.h"
#include "../include/SD.h"
#include "../include/NVIC.h"
#include "../include/USART.h"

#include <string.h>
#include <stdint.h>

DMA_Request dma_queue[DMA1_QUEUE_SIZE];
uint8_t dma_write_ptr = 0;
uint8_t dma_read_ptr = 0;
uint8_t dummy_rx = 0xFF;
uint8_t dummy_tx = 0xFF;

void DMA1_SD_transmit(uint8_t* tx, uint8_t* rx)
{
    uint8_t* transfer = tx != NULL? tx : &dummy_tx;
    uint8_t* recieve = rx != NULL? rx : &dummy_rx;

    DMA1_BASE->CHNL_REGS[1].CCR = 0;
    DMA1_BASE->CHNL_REGS[2].CCR = 0;
    
    DMA1_BASE->CHNL_REGS[1].CCR |= 2 << 12 | 
                            (rx != NULL) << 7 |
                             0 << 4 | 
                             5 << 1;
    DMA1_BASE->CHNL_REGS[1].CNDTR = BLOCK_SIZE + 2;
    DMA1_BASE->CHNL_REGS[1].CPAR = (uint32_t)&(SPI1_BASE->DR);
    DMA1_BASE->CHNL_REGS[1].CMAR = (uint32_t)recieve;

    DMA1_BASE->CHNL_REGS[2].CCR |= 2 << 12 | 
                            (tx != NULL) << 7 |
                             1 << 4 | 
                             1 << 3;
    DMA1_BASE->CHNL_REGS[2].CPAR = (uint32_t)&(SPI1_BASE->DR);
    DMA1_BASE->CHNL_REGS[2].CMAR = (uint32_t)transfer;  
    DMA1_BASE->CHNL_REGS[2].CNDTR = BLOCK_SIZE + 2;
}

void DMA_init()
{
    RCC_BASE_ADDR->AHB_ENBR |= 1 << 0;
}

void DMA1_enable()
{
    DMA1_BASE->CHNL_REGS[1].CCR |= (1 << 0);
    DMA1_BASE->CHNL_REGS[2].CCR |= (1 << 0);
    SPI1_BASE->CR2 |= (3 << 0);
    NVIC_enable_irq(DMA1_IRQ);
}

void DMA1_disable()
{
    DMA1_BASE->IFCR |= 1 << 5;
    DMA1_BASE->IFCR |= 1 << 9;
    DMA1_BASE->CHNL_REGS[1].CCR &= ~(1 << 0);
    DMA1_BASE->CHNL_REGS[2].CCR &= ~(1 << 0);
    SPI1_BASE->CR2 &= ~(3 << 0);
}


void DMA1_complete_current()
{
    uint8_t index = dma_read_ptr % DMA1_QUEUE_SIZE;
    if(dma_queue[index].dma_op == CMD_SD_READ)
    {
        memcpy(dma_queue[index].buff, sector_buff, dma_queue[index].size);
    }
    else
    {
        uint8_t response = SPI_transmit_poll(sd_base, 0xFF) & 0x1F;
        if(response != 0x05) USART1_write_line("write rejected!\r\n");
    }
    dma_queue[index].done = 1;
    dma_read_ptr++;
}


uint8_t DMA1_start_next()
{
    if(dma_read_ptr == dma_write_ptr) return 0;

    uint8_t index = dma_read_ptr % DMA1_QUEUE_SIZE;

    if(dma_queue[index].dma_op == CMD_SD_WRITE)
    {
        memset(sector_buff, 0, BLOCK_SIZE);
        memcpy(sector_buff, dma_queue[index].buff, dma_queue[index].size);
    }

    CS_low();
    SD_send_command(dma_queue[index].dma_op, dma_queue[index].lba, 0xFF);

    uint8_t status = SD_get_response();
    if(status != 0)
    {
        USART1_write_line("cmd error!\r\n");
        CS_high();
        return 0;
    }

    if(dma_queue[index].dma_op == CMD_SD_WRITE)
    {
        SPI_transmit_poll(sd_base, 0xFE);          
        DMA1_SD_transmit(sector_buff, NULL);
    }
    else
    {
        while(SPI_transmit_poll(sd_base, 0xFF) != 0xFE);
        DMA1_SD_transmit(NULL, sector_buff);
    }

    return 1;
}

void DMA1_interrupt_handler()
{
    if(DMA1_BASE->ISR & ((1 << 7) | (1 << 11)))
    {
        USART1_write_line("DMA ERROR\r\n");
        DMA1_disable();
        CS_high();
        return;
    }

    DMA1_disable();

    DMA1_complete_current();
    CS_high();
    if(DMA1_start_next())
        DMA1_enable();
}

