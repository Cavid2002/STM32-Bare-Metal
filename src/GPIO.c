#include "../include/GPIO.h"


void GPIO_pinMode(GPIO_REGS* base, uint32_t pin, uint32_t mode, uint32_t cfg)
{
    uint32_t pin_offset = pin % 8;
    uint32_t mode_cfg = (cfg << 2) | mode;

    if (pin < 8) 
    {
        base->CFGR_LOW &= ~(0xF << (pin_offset << 2));
        base->CFGR_LOW |=  mode_cfg << (pin_offset << 2);
    } 
    else 
    {
        base->CFGR_HIGH &= ~(0xF << (pin_offset << 2));
        base->CFGR_HIGH |=  mode_cfg << (pin_offset << 2);
    }
}


void GPIO_pinWrite(GPIO_REGS* base, uint32_t pin, uint32_t val)
{
    if (val)
        base->ODR |= (1 << pin); 
    else
        base->ODR &= ~(1 << pin); 
}

void GPIO_pinToggle(GPIO_REGS* base, uint32_t pin)
{
    base->ODR ^= 1 << pin;
}

uint32_t GPIO_pinRead(GPIO_REGS* base, uint32_t pin)
{
    return (base->IDR >> pin) & 1;
}

void GPIO_pinSet(GPIO_REGS* base, uint32_t pin)
{
    base->BSRR = 1 << pin;
}

void GPIO_pinReset(GPIO_REGS* base, uint32_t pin)
{
    base->BRR = 1 << pin;
}

void GPIO_pinLock(GPIO_REGS* base, uint32_t pin)
{
    base->LCKR |= 1 << pin;
}

void GPIO_pinUnlock(GPIO_REGS* base, uint32_t pin)
{
    base->LCKR &= ~(1 << pin);
}