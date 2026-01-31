#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#define RCC_APB1_ENB_TIM2       0
#define RCC_APB1_ENB_TIM3       1
#define RCC_APB1_ENB_TIM4       2
#define RCC_APB1_ENB_TIM5       3
#define RCC_APB1_ENB_TIM6       4
#define RCC_APB1_ENB_TIM7       5
#define RCC_APB1_ENB_TIM12      6
#define RCC_APB1_ENB_TIM13      7
#define RCC_APB1_ENB_TIM14      8
#define RCC_APB1_ENB_WWD        11
#define RCC_APB1_ENB_SPI2       14
#define RCC_APB1_ENB_SPI3       15
#define RCC_APB1_ENB_USART2     17
#define RCC_APB1_ENB_USART3     18
#define RCC_APB1_ENB_USART4     19
#define RCC_APB1_ENB_USART5     20
#define RCC_APB1_ENB_I2C1       21
#define RCC_APB1_ENB_I2C2       22
#define RCC_APB1_ENB_USB        23
#define RCC_APB1_ENB_CAN        25
#define RCC_APB1_ENB_BKP        27
#define RCC_APB1_ENB_PWRI       28
#define RCC_APB1_ENB_DAC        29




#define RCC_APB2_ENB_AFIO       0
#define RCC_APB2_ENB_PORT_A     2
#define RCC_APB2_ENB_PORT_B     3
#define RCC_APB2_ENB_PORT_C     4
#define RCC_APB2_ENB_PORT_D     5
#define RCC_APB2_ENB_PORT_E     6
#define RCC_APB2_ENB_PORT_F     7
#define RCC_APB2_ENB_PORT_G     8
#define RCC_APB2_ENB_ADC1       9
#define RCC_APB2_ENB_ADC2       10
#define RCC_APB2_ENB_TIM1       11
#define RCC_APB2_ENB_SPI1       12
#define RCC_APB2_ENB_TIM8       13
#define RCC_APB2_ENB_USART1     14
#define RCC_APB2_ENB_ADC3       15
#define RCC_APB2_ENB_TIM9       19
#define RCC_APB2_ENB_TIM10      20 
#define RCC_APB2_ENB_TIM11      21


#define RCC_AHB_ENB_DMA1        0
#define RCC_AHB_ENB_DMA2        1
#define RCC_AHB_ENB_SRAM        2
#define RCC_AHB_ENB_FLASH       4
#define RCC_AHB_ENB_CRC         6
#define RCC_AHB_ENB_FSMC        8
#define RCC_AHB_ENB_SDIO        10


#define RCC_CR_PLLRDY   25
#define RCC_CR_PLLEN    24
#define RCC_CR_CSEN     19
#define RCC_CR_HSEBYP   18
#define RCC_CR_HSERDY   17
#define RCC_CR_HSEEN    16
#define RCC_CR_HSICAL   8
#define RCC_CR_HSITRIM  3
#define RCC_CR_HSIRDY   1
#define RCC_CR_HSIEN    0

#define RCC_CFGR_SW     0


#define RCC_SW_HSI      0
#define RCC_SW_HSE      1
#define RCC_SW_PLL      2





typedef struct{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2_RSTR;
    volatile uint32_t APB1_RSTR;
    volatile uint32_t AHB_ENBR;
    volatile uint32_t APB2_ENBR;
    volatile uint32_t APB1_ENBR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
}RCC_REGS;


void rcc_set();

void RCC_APB1DevEnable(uint32_t dev);
void RCC_APB2DevEnable(uint32_t dev);
void RCC_AHBDevEnable(uint32_t dev);


void RCC_APB1DevDisable(uint32_t dev);
void RCC_APB2DevDisable(uint32_t dev);
void RCC_AHBDevDisable(uint32_t dev);


int RCC_HSE_PLL_enable();

#define RCC_BASE_ADDR ((RCC_REGS*)0x40021000)
#endif