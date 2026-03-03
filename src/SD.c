#include "../include/SPI.h"
#include "../include/USART.h"
#include "../include/SD.h"



SPI_REGS* sd_base = SPI1_BASE;
USART_REGS* usart_log = USART1_BASE;

void console_log(const char* msg)
{
    USART_write_line(USART1_BASE, msg);
    USART_write_line(USART1_BASE, "\r\n");
}

uint8_t SD_send_command(uint8_t cmd, uint32_t args, uint8_t crc)
{
    uint8_t res = 0;
    console_log("Sending Command");

    SPI_transmit_poll(sd_base, 0x40 | cmd);
    SPI_transmit_poll(sd_base, (args >> 24) & 0xFF);
    SPI_transmit_poll(sd_base, (args >> 16) & 0xFF);
    SPI_transmit_poll(sd_base, (args >> 8) & 0xFF);
    SPI_transmit_poll(sd_base, args & 0xFF);
    SPI_transmit_poll(sd_base, crc | 0x01);
    for(int i = 0; i <= SD_NCR; i++)
    {
        res = SPI_transmit_poll(sd_base, 0xFF);
        if(!(res & 0x80)) return res; 
    }
    
    console_log("COMMAND FAILED!");
    return 0xFF;
}


int SD_reset()
{
    for(int i = 0; i < 10000000; i++);
    uint8_t status;
    char buf[32];
    console_log("SD RESET start...");
    SD_adjust_freq(6);
    CS_high();
    for(int i = 0; i < 100; i++)
        SPI_transmit_poll(sd_base, 0xFF);

    CS_low();
    SPI_transmit_poll(sd_base, 0xFF);
    status = SD_send_command(0, 0, 0x95);

    // print the raw byte so we can see what came back
    buf[0] = '0';
    buf[1] = 'x';
    buf[2] = "0123456789ABCDEF"[(status >> 4) & 0xF];
    buf[3] = "0123456789ABCDEF"[status & 0xF];
    buf[4] = '\0';
    console_log("CMD0 response: ");
    console_log(buf);
    if(status != 0x01)
    {
        console_log("[ERROR] SD reset failed");
        CS_high();
        return -1;
    }

    console_log("[SUCCESS] SD reset completed");
    SD_adjust_freq(0);
    CS_high();
    return 0;
}



int SD_init()
{

}

int SD_read_block(char* buff, uint32_t lba)
{
    CS_low();
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
    CS_high();
    return res;
}

int SD_write_block(char* buff, uint32_t lba)
{
    CS_low();
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
    CS_high();
    return res;
}

void SD_adjust_freq(uint8_t freq)
{
    freq &= 0x07;
    sd_base->CR1 &= ~(1 << 6);
    sd_base->CR1 &= ~(7 << 3);
    sd_base->CR1 |= freq << 3;
    sd_base->CR1 |= (1 << 2) | (1 << 9) | (1 << 8);
    sd_base->CR1 |= 1 << 6;
}
