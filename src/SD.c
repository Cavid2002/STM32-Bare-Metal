#include "../include/SPI.h"
#include "../include/USART.h"
#include "../include/SD.h"



SPI_REGS* sd_base = SPI1_BASE;
USART_REGS* usart_log = USART1_BASE;
uint8_t sd_card_type;

void console_log(const char* msg)
{
    USART_write_line(USART1_BASE, msg);
    USART_write_line(USART1_BASE, "\r\n");
}

void SD_send_command(uint8_t cmd, uint32_t args, uint8_t crc)
{
    while(SPI_transmit_poll(sd_base, 0xFF) != 0xFF);
    SPI_transmit_poll(sd_base, 0x40 | cmd);
    SPI_transmit_poll(sd_base, (args >> 24) & 0xFF);
    SPI_transmit_poll(sd_base, (args >> 16) & 0xFF);
    SPI_transmit_poll(sd_base, (args >> 8) & 0xFF);
    SPI_transmit_poll(sd_base, args & 0xFF);
    SPI_transmit_poll(sd_base, crc | 0x01);
}



uint8_t SD_get_response()
{
    uint8_t res;
    for(int i = 0; i <= SD_NCR; i++)
    {
        res = SPI_transmit_poll(sd_base, 0xFF);
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
        SPI_transmit_poll(sd_base, 0xFF);


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

    if (SD_reset() != 0)
    {
        console_log("[ERROR] SD init failed at reset");
        return -1;
    }

    /* CMD8 - Send Interface Condition (detect SDv2) */
    CS_low();
    SD_send_command(8, 0x1AA, 0x87);
    status = SD_get_response();

    if (status == 0x01)
    {
        /* SDv2 - read remaining 4 bytes of R7 response */
        ocr[0] = SPI_transmit_poll(sd_base, 0xFF);
        ocr[1] = SPI_transmit_poll(sd_base, 0xFF);
        ocr[2] = SPI_transmit_poll(sd_base, 0xFF);
        ocr[3] = SPI_transmit_poll(sd_base, 0xFF);
        CS_high();

        if ((ocr[2] != 0x01) || (ocr[3] != 0xAA))
        {
            console_log("[ERROR] SD voltage range mismatch");
            return -1;
        }

        /* ACMD41 loop - wait for card to leave idle (SDv2, HCS=1) */
        timeout = 0;
        do {
            CS_low();
            SD_send_command(55, 0, 0x65);   /* CMD55 - APP_CMD */
            status = SD_get_response();
            CS_high();

            if (status > 0x01)
            {
                console_log("[ERROR] CMD55 failed");
                return -1;
            }

            CS_low();
            SD_send_command(41, 0x40000000, 0x77);  /* ACMD41 HCS=1 */
            status = SD_get_response();
            CS_high();

            if (++timeout > SD_TIMEOUT)
            {
                console_log("[ERROR] SD ACMD41 timeout (SDv2)");
                return -1;
            }

        } while (status == 0x01);

        /* CMD58 - Read OCR, check CCS bit for SDHC/SDXC */
        CS_low();
        SD_send_command(58, 0, 0xFD);
        status = SD_get_response();
        ocr[0] = SPI_transmit_poll(sd_base, 0xFF);
        ocr[1] = SPI_transmit_poll(sd_base, 0xFF);
        ocr[2] = SPI_transmit_poll(sd_base, 0xFF);
        ocr[3] = SPI_transmit_poll(sd_base, 0xFF);
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
        /* SDv1 or MMC - ACMD41 without HCS */
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

    /* CMD16 - Force block size to 512 (required for SDSC) */
    if (sd_card_type == SD_TYPE_SDSC)
    {
        CS_low();
        SD_send_command(16, SECTOR_SIZE, 0xFF);
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

int SD_read_block(char* buff, uint32_t lba)
{
    uint8_t status;
    uint32_t timeout = SD_TIMEOUT;
    CS_low();
    SD_send_command(17, lba, 0xFF);
    status = SD_get_response();
    if(status != 0)
    {
        console_log("Read error!");
        CS_high();
        return -1;
    } 

    while(1)
    {
        if(SPI_transmit_poll(sd_base, 0xFF) == 0xFE) break;
        if(!timeout)
        {
            CS_high();
            console_log("Data Token error!");
            return -1;
        }
        timeout--;
    }

    for(int i = 0; i < SECTOR_SIZE; i++)
    {
        buff[i] = SPI_transmit_poll(sd_base, 0xFF);
    }

    SPI_transmit_poll(sd_base, 0xFF);
    SPI_transmit_poll(sd_base, 0xFF);

    CS_high();
    return 512;
}

int SD_write_block(char* buff, uint32_t lba)
{
    uint8_t status;
    uint32_t timeout = SD_TIMEOUT;
    CS_low();
    SD_send_command(24, lba, 0xFF);
    status = SD_get_response();
    if(status != 0)
    {
        console_log("write error!");
        CS_high();
        return -1;
    } 


    SPI_transmit_poll(sd_base, 0xFE);
    for(int i = 0; i < SECTOR_SIZE; i++)
    {
        SPI_transmit_poll(sd_base, buff[i]);
    }

    SPI_transmit_poll(sd_base, 0xFF);
    SPI_transmit_poll(sd_base, 0xFF);
    
    status = SPI_transmit_poll(sd_base, 0xFF);

    if((status & 0x0F) != 0x05)
    {
        CS_high();
        console_log("Write error");
        return -1;
    }
    
    CS_high();
    return 512;
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
