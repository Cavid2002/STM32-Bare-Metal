#ifndef SPI_H
#define SPI_H

#include <stdint.h>

#define SPI_SR_BSY      1 << 7
#define SPI_SR_OVR      1 << 6
#define SPI_SR_MODF     1 << 5
#define SPI_SR_CRCERR   1 << 4
#define SPI_SR_TXE      1 << 1
#define SPI_SR_RXNE     1 << 0


typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCSPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
} SPI_REGS;

#define SPI1_BASE ((SPI_REGS*)(0x40013000))


uint8_t SPI_transmit(SPI_REGS* base, uint8_t data);
uint8_t SPI_read_poll(SPI_REGS* base);
void SPI_write_poll(SPI_REGS* base, uint8_t data);
void SPI_start(SPI_REGS* base);
void SPI_stop(SPI_REGS* base);

#endif