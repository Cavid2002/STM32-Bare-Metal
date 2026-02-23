#include "../include/SPI.h"
#include "../include/USART.h"
#include "../include/SD.h"



SPI_REGS* sd_base = SPI1_BASE;


int SD_reset()
{
    uint8_t status;
    CS_high(sd_base);
    for(int i = 0; i < 10; i++)
    {
        SPI_transmit_poll(sd_base, 0xFF);
    }
    
    CS_low(sd_base);

    status = SD_send_command(0, 0, 0x95);
    if(status != 0x01)
    {
        USART_write_line(USART1_BASE, "[ERROR] SD init failed");
        return -1;
    }
    
    return 0;
}



uint8_t SD_send_command(uint8_t cmd, uint32_t args, uint8_t crc)
{
    uint8_t res = 0;
    SPI_transmit_poll(sd_base, 0x40 | cmd);
    SPI_transmit_poll(sd_base, (args >> 24) & 0xFF);
    SPI_transmit_poll(sd_base, (args >> 16) & 0xFF);
    SPI_transmit_poll(sd_base, (args >> 8) & 0xFF);
    SPI_transmit_poll(sd_base, args & 0xFF);

    for(int i = 0; i < SD_NCR; i++)
    {
        res = SPI_transmit_poll(sd_base, 0xFF);
        if(!(res & 0x80)) return res; 
    }
    USART_write_line(USART1_BASE, "COMMAND FAILED \r\n");
    return 0xFF;
}

int SD_init()
{
    SD_adjust_freq(6);
    CS_low(sd_base);
    uint8_t res, timeout = TIMEOUT;

    if(SD_reset() < 0)
    {
        USART_write_line(USART1_BASE, "[ERROR] SD init failed");
        return -1;
    }

    while(1)
    {
        SD_send_command(55, 0, 0);
        res = SD_send_command(41, 0x40000000, 0xFF);
        if(res == 0) break;
        if(timeout == 0)
        {
            USART_write_line(USART1_BASE, "[ERROR] SD init failed");
            return -1;
        }
        timeout--;
        
    }

    CS_high(sd_base);
    SD_adjust_freq(0);
    return 0;

}

int SD_read_block(char* buff, uint32_t lba)
{
    CS_low(sd_base);
    int timeout = TIMEOUT, res = 0;
    uint8_t status = SD_send_command(17, lba, 0xFF);
    if(status == 0xFF) return -1;
    
    while(1)
    {
        if(timeout == 0) return -1;
        if(SPI_transmit_poll(sd_base, 0xFF) == DATA_TOKEN) break;
        timeout--;
    }
    
    for(int i = 0; i < SECTOR_SIZE; i++)
    {
        buff[i] = SPI_transmit_poll(sd_base, 0xFF);
        res++;
    }

    SPI_transmit_poll(sd_base, 0xFF);
    SPI_transmit_poll(sd_base, 0xFF);
    CS_high(sd_base);
    return res;
}

int SD_write_block(char* buff, uint32_t lba)
{
    CS_low(sd_base);
    int timeout = TIMEOUT, res = 0;
    uint8_t status = SD_send_command(24, lba, 0xFF);
    if(status == 0xFF) return -1;

    SPI_transmit_poll(sd_base, DATA_TOKEN);
    
    for(int i = 0; i < SECTOR_SIZE; i++)
    {
        SPI_transmit_poll(sd_base, buff[i]);
        res++;
    }
    
    SPI_transmit_poll(sd_base, 0xFF);
    SPI_transmit_poll(sd_base, 0xFF);

    while(SPI_transmit_poll(sd_base, 0xFF) == 0x00);

    status = SPI_transmit_poll(sd_base, 0xFF);
    if((status & 0x1F) != 0x05) return -1;
    CS_high(sd_base);
    return res;
}

void SD_adjust_freq(uint8_t freq)
{
    freq &= 0x07;
    sd_base->CR1 &= ~(1 << 6);
    sd_base->CR1 &= ~(7 << 3);
    sd_base->CR1 |= freq << 3;
    sd_base->CR1 |= 1 << 6;
}
