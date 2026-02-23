#ifndef GPIO_H
#define GPIO_H


#include <stdint.h>

#define GPIO_CFG_INPUT_ANLG         0x0
#define GPIO_CFG_INPUT_FLT          0x1
#define GPIO_CFG_INPUT_PUSH_PULL    0x2
#define GPIO_CFG_INPUT_RESERVED     0x3

#define GPIO_CFG_OUTPUT_PUSH_PULL   0x0
#define GPIO_CFG_OUTPUT_OPEN_DRAIN  0x1
#define GPIO_CFG_ALTR_PUSH_PULL     0x2
#define GPIO_CFG_ALTR_OPEN_DRAIN    0x3

#define GPIO_MODE_INPUT             0x0
#define GPIO_MODE_OUTPUT_10Mhz      0x1
#define GPIO_MODE_OUTPUT_2Mhz       0x2
#define GPIO_MODE_OUTPUT_50Mhz      0x3



typedef struct
{
    volatile uint32_t CFGR_LOW;
    volatile uint32_t CFGR_HIGH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_REGS;

#define GPIO_BASE_A ((GPIO_REGS*)0x40010800)
#define GPIO_BASE_B ((GPIO_REGS*)0x40010C00)
#define GPIO_BASE_C ((GPIO_REGS*)0x40011000)

void GPIOC_init();
void blink_led();


void GPIO_pinMode(GPIO_REGS* base, uint32_t pin, uint32_t mode, uint32_t cfg);
void GPIO_pinWrite(GPIO_REGS* base, uint32_t pin, uint32_t val);
void GPIO_pinToggle(GPIO_REGS* base, uint32_t pin);
void GPIO_pinSet(GPIO_REGS* base, uint32_t pin);
void GPIO_pinReset(GPIO_REGS* base, uint32_t pin);
void GPIO_pinLock(GPIO_REGS* base, uint32_t pin);
void GPIO_pinUnlock(GPIO_REGS* base, uint32_t pin);
uint32_t GPIO_pinRead(GPIO_REGS* base, uint32_t pin);

#endif
