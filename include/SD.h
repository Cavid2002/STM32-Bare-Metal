#ifndef SD_H
#define SD_H

#include <stdint.h>
#include "../include/SPI.h"

#define TIMEOUT 100
#define SD_TIMEOUT  5000

#define SD_TYPE_SDSC     0
#define SD_TYPE_SDHC     1


#define MAX_RESPONSE_SIZE  6

#define SD_NCR       8
#define DATA_TOKEN   0xFE
#define BLOCK_SIZE   512
#define FREQ_400Khz  6
#define FREQ_18Mhz  1


extern uint8_t sector_buff[];
extern SPI_REGS* sd_base;
int SD_init();
int SD_write_block(char* buff, uint32_t lba);
int SD_read_block(char* buff, uint32_t lba);
void SD_adjust_freq(uint8_t freq);
int SD_reset();
void SD_send_command(uint8_t cmd, uint32_t args, uint8_t crc);
void console_log(const char* msg);
uint8_t SD_get_response();
int SD_begin();

void SPI_loopback_test();
#endif