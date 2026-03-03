#ifndef SD_H
#define SD_H

#include <stdint.h>
#include "../include/SPI.h"
#define TIMEOUT 100

#define SD_TYPE_SDSC 0
#define SD_TYPE_SDHC 1
#define SD_TYPE_SDXC 2

#define SD_NCR       8
#define DATA_TOKEN   0xFE
#define SECTOR_SIZE  512
#define FREQ_400Khz  6
#define FREQ_18Mhz  1

extern SPI_REGS* sd_base;
int SD_init();
int SD_write_block(char* buff, uint32_t lba);
int SD_read_block(char* buff, uint32_t lba);
void SD_adjust_freq(uint8_t freq);
int SD_reset();
uint8_t SD_send_command(uint8_t cmd, uint32_t args, uint8_t crc);
void console_log(const char* msg);

void SPI_loopback_test();
#endif