#ifndef SPI_H
#define SPI_H

#include <stdint.h>

#define SPI_SR_BSY      (1 << 7)
#define SPI_SR_OVR      (1 << 6)
#define SPI_SR_MODF     (1 << 5)
#define SPI_SR_CRCERR   (1 << 4)
#define SPI_SR_TXE      (1 << 1)
#define SPI_SR_RXNE     (1 << 0)


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
#define SPI2_BASE ((SPI_REGS*)(0x40003800))

// SD card
void SPI1_init();
uint8_t SPI_transmit_poll(SPI_REGS* base, uint8_t data);
void CS_low();
void CS_high();


// LCD screen 
void SPI2_init();
void DC_low();
void DC_high();
void SPI2_send_poll(uint8_t data);
void CS2_low();
void CS2_high();
void RST_low();
void RST_high();

#endif