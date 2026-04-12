#include "../include/SPI.h"
#include "../include/USART.h"
#include "../include/SD.h"
#include "../include/DMA.h"
#include "../include/SpinLock.h"
#include <stddef.h>

uint8_t sector_buff[BLOCK_SIZE + 2] = {[512] = 0xFF, [513] = 0xFF};
SPI_REGS* sd_base = SPI1_BASE;
USART_REGS* usart_log = USART1_BASE;
uint8_t sd_card_type;

void console_log(const char* msg)
{
    USART1_write_line(msg);
    USART1_write_line("\r\n");
}

void SD_send_command(uint8_t cmd, uint32_t args, uint8_t crc)
{
    while(SPI_transmit_poll(SPI1_BASE, 0xFF) != 0xFF);
    SPI_transmit_poll(SPI1_BASE, 0x40 | cmd);
    SPI_transmit_poll(SPI1_BASE, (args >> 24) & 0xFF);
    SPI_transmit_poll(SPI1_BASE, (args >> 16) & 0xFF);
    SPI_transmit_poll(SPI1_BASE, (args >> 8) & 0xFF);
    SPI_transmit_poll(SPI1_BASE, args & 0xFF);
    SPI_transmit_poll(SPI1_BASE, crc | 0x01);
}



uint8_t SD_get_response()
{
    uint8_t res;
    for(int i = 0; i <= SD_NCR; i++)
    {
        res = SPI_transmit_poll(SPI1_BASE, 0xFF);
        if(!(res & 0x80)) return res;
         
    }
    return 0xFF;
}


int SD_reset()
{
    SD_adjust_freq(7);
    for(int i = 0; i < 100000; i++);
    uint8_t status;
    console_log("SD RESET start...");
    CS_high();
    for(int i = 0; i < 10; i++)
        SPI_transmit_poll(SPI1_BASE, 0xFF);


    CS_low();
    SD_send_command(0, 0, 0x95);
    status = SD_get_response();
    CS_high();
    if(status != 0x01)
    {
        console_log("[ERROR] SD reset failed");
        return -1;
    }

    console_log("[SUCCESS] SD reset completed");
    SD_adjust_freq(1);
    return 0;
}



int SD_init()
{
    uint8_t status;
    uint8_t ocr[4];
    uint32_t timeout;

    console_log("SD INIT start...");
    CS_low();
    SD_send_command(8, 0x1AA, 0x87);
    status = SD_get_response();

    if (status == 0x01)
    {
        ocr[0] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        ocr[1] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        ocr[2] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        ocr[3] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        CS_high();

        if ((ocr[2] != 0x01) || (ocr[3] != 0xAA))
        {
            console_log("[ERROR] SD voltage range mismatch");
            return -1;
        }

        timeout = 0;
        do {
            CS_low();
            SD_send_command(55, 0, 0x65);
            status = SD_get_response();
            CS_high();

            if (status > 0x01)
            {
                console_log("[ERROR] CMD55 failed");
                return -1;
            }

            CS_low();
            SD_send_command(41, 0x40000000, 0x77);
            status = SD_get_response();
            CS_high();

            if (++timeout > SD_TIMEOUT)
            {
                console_log("[ERROR] SD ACMD41 timeout (SDv2)");
                return -1;
            }

        } while (status == 0x01);

        CS_low();
        SD_send_command(58, 0, 0xFD);
        status = SD_get_response();
        ocr[0] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        ocr[1] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        ocr[2] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        ocr[3] = SPI_transmit_poll(SPI1_BASE, 0xFF);
        CS_high();

        if (status != 0x00)
        {
            console_log("[ERROR] CMD58 failed");
            return -1;
        }

        sd_card_type = (ocr[0] & 0x40) ? SD_TYPE_SDHC : SD_TYPE_SDSC;
    }
    else
    {
        CS_high();
        timeout = 0;
        do {
            CS_low();
            SD_send_command(55, 0, 0x65);
            status = SD_get_response();
            CS_high();

            CS_low();
            SD_send_command(41, 0, 0xE5);
            status = SD_get_response();
            CS_high();

            if (++timeout > SD_TIMEOUT)
            {
                console_log("[ERROR] SD ACMD41 timeout (SDv1)");
                return -1;
            }

        } while (status == 0x01);

        sd_card_type = SD_TYPE_SDSC;
    }

    if (sd_card_type == SD_TYPE_SDSC)
    {
        CS_low();
        SD_send_command(16, BLOCK_SIZE, 0xFF);
        status = SD_get_response();
        CS_high();

        if (status != 0x00)
        {
            console_log("[ERROR] CMD16 block size set failed");
            return -1;
        }
    }

    console_log("[SUCCESS] SD init completed");
    return 0;
}

int SD_begin()
{
    if(SD_reset() < 0)
    {
        console_log("SD Reset Failed!");
        return -1;    
    }
    if(SD_init() < 0)
    {
        console_log("SD INIT Failed!");
        return -1;  
    }
    return 0;
}

int SD_write_sync(char* buff, uint32_t lba, uint16_t size)
{
    while(!SDA_dma_enque(buff, lba, size, CMD_SD_WRITE));

    sched_block();    
    return size;
}

int SD_read(char* buff, uint32_t lba, uint16_t size)
{
    while(!SDA_dma_enque(buff, lba, size, CMD_SD_READ));

    sched_block();

    return size;
}

int SD_write(char* buff, uint32_t lba, uint16_t  size)
{
    while(!SDA_dma_enque(buff, lba, size, CMD_SD_WRITE));

    return size;
}

int SDA_dma_enque(char* buff, uint32_t lba, uint16_t size, uint8_t op)
{
    _DISABLE_INTR();
    if(dma_write_ptr - dma_read_ptr >= DMA1_QUEUE_SIZE) return 0;
    if(size > 512) size = 512;
    uint8_t index = dma_write_ptr % DMA1_QUEUE_SIZE;

    
    dma_queue[index].buff = buff;
    dma_queue[index].size = size;
    dma_queue[index].lba = lba;
    dma_queue[index].dma_op = op;
    dma_queue[index].done = 0;
    dma_queue[index].waiting_task = (op == CMD_SD_READ) ? current_task : NULL;
    dma_write_ptr++;

    if(dma_write_ptr - dma_read_ptr == 1)
    {
        if(DMA1_start_next())
        {
            DMA1_enable();
        }
            
    }
    _ENABLE_INTR();
    return 1;
}



void SD_adjust_freq(uint8_t freq)
{
    freq &= 0x07;
    SPI1_BASE->CR1 &= ~(1 << 6);
    SPI1_BASE->CR1 &= ~(7 << 3);
    SPI1_BASE->CR1 |= freq << 3;
    SPI1_BASE->CR1 |= (1 << 2) | (1 << 9) | (1 << 8);
    SPI1_BASE->CR1 |= 1 << 6;
}
