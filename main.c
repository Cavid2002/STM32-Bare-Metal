#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"
#include "./include/LCD.h"  
#include "./include/DMA.h"
#include "./include/STK.h"
#include "./include/Task.h"
#include "./include/SpinLock.h"
#include <string.h>

volatile uint32_t lock = 0;
volatile uint32_t test = 0;
volatile uint32_t task_a_done = 0;
volatile uint32_t task_b_done = 0;


void task_a()
{
    for(int i = 0; i < 1000000; i++)
    {
        spinlock_acquire(&lock);
        test++;
        spinlock_release(&lock);
    }

    task_a_done = 1;
        
    while(1);
}

void task_b()
{
    for(int i = 0; i < 1000000; i++)
    {
        spinlock_acquire(&lock);
        test++;
        spinlock_release(&lock);
    }
    task_b_done = 1;
        
    while(1);
}

void task_c()
{
    while(task_a_done != 1 || task_b_done != 1);
    USART1_print_number(test);
    USART1_write_line("\r\n");
    while(1);
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
    }
}


