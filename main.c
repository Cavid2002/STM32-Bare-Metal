#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"
#include "./include/LCD.h"
#include "./include/DMA.h"
#include "./include/STK.h"
#include "./include/Task.h"
#include <string.h>


char start[30];
char temp[30];

void delay(uint32_t delay)
{
    while(delay)
    {
        delay--;
    }
}


void task_a()
{
    for(volatile int i = 0; i < 10; i++)
    USART1_write_line("Task A running\r\n");
    while(1)
    {
        
    }
}

void task_b()
{
    for(volatile int i = 0; i < 10; i++)
        USART1_write_line("Task B running\r\n");
    while(1);
}

void task_c()
{
    for(volatile int i = 0; i < 10; i++)
        USART1_write_line("Task C running\r\n");
    while(1)
    {
        
    }
}


int main()
{
    uint8_t key = 0, counter = 0;
    RCC_HSE_PLL_enable();
    RCC_APB2DevEnable(RCC_APB2_ENB_PORT_A);
    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_A, 1, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinToggle(GPIO_BASE_A, 1);    
    
    USART1_init(115200);
    USART1_interrupt_enable();

    sched_init();

    sched_task_create(task_a);
    sched_task_create(task_b);
    sched_task_create(task_c);

    sched_enable();
    
    
    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 0);
        delay(1000000);
    }
}


