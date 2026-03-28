#include "../include/DMA.h"
#include "../include/RCC.h"
#include "../include/SD.h"
#include <stdint.h>


void DMA1_SD_read()
{
    RCC_BASE_ADDR->AHB_ENBR |= 1 << 0;
    DMA1_BASE->CHNL_REGS[1].CCR |= 2 << 12;
    DMA1_BASE->CHNL_REGS[1].CCR |= 1 << 7;
    DMA1_BASE->CHNL_REGS[1].CCR |= 7 << 1;

    DMA1_BASE->CHNL_REGS[1].CNDTR = BLOCK_SIZE;
    DMA1_BASE->CHNL_REGS[1].CPAR = &(SPI1_BASE->DR);
    DMA1_BASE->CHNL_REGS[1].CMAR = sector_buff;
}

void DMA1_SD_write()
{
    DMA1_BASE->CHNL_REGS[1].CCR |= 1 << 0;
    
}

