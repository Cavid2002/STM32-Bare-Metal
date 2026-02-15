#include "../include/SD.h"
#include "../include/SPI.h"
#include "../include/USART.h"

void SD_reset(SPI_REGS* base)
{
    uint8_t status;
    SPI_stop(base);
    for(int i = 0; i < 10; i++)
    {
        SPI_transmit(0xFF);
    }
    
    SPI_start(base);

    for(int i = 0; i < 6; i++)
    {
        status = SPI_transmit(CMD0[i]);  
    } 
    
    if(status != 0x01)
    {
        USART_write_line(USART1_BASE, "[ERROR] SD init failed");
        return -1;
    }

    

}

void SD_init()
{
    
}