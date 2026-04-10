#ifndef SPI_H
#define SPI_H

#include <stdint.h>

#define SPI_SR_BSY      (1 << 7)
#define SPI_SR_OVR      (1 << 6)
#define SPI_SR_MODF     (1 << 5)
#define SPI_SR_CRCERR   (1 << 4)
#define SPI_SR_TXE      (1 << 1)
#define SPI_SR_RXNE     (1 << 0)


#define SPI_CR1_CPHA        (1 << 0)  
#define SPI_CR1_CPOL        (1 << 1)  
#define SPI_CR1_MSTR        (1 << 2)  
#define SPI_CR1_BR_0        (1 << 3)  
#define SPI_CR1_BR_1        (1 << 4)  
#define SPI_CR1_BR_2        (1 << 5)  
#define SPI_CR1_SPE         (1 << 6)  
#define SPI_CR1_LSBFIRST    (1 << 7)   
#define SPI_CR1_SSI         (1 << 8)  
#define SPI_CR1_SSM         (1 << 9)  
#define SPI_CR1_RXONLY      (1 << 10) 
#define SPI_CR1_DFF         (1 << 11)
#define SPI_CR1_CRCNEXT     (1 << 12) 
#define SPI_CR1_CRCEN       (1 << 13) 
#define SPI_CR1_BIDIOE      (1 << 14) 
#define SPI_CR1_BIDIMODE    (1 << 15)

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