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

__attribute__((aligned(4))) lock_t lock = 0;
uint32_t test = 0;
uint32_t task_a_done = 0;
uint32_t task_b_done = 0;

void delay(uint32_t delay)
{
    while(delay--);
}

void task_a()
{
    
    for(int i = 0; i < 10000; i++)
    {
        mutex_acquire(&lock);
        test++;
        delay(100);
        mutex_release(&lock);
    }

    task_a_done = 1;
        
    while(1)
    {
       
    }
}

void task_b()
{
    
    for(int i = 0; i < 10000; i++)
    {
        mutex_acquire(&lock);
        test++;
        delay(100);
        mutex_release(&lock);
    }

    task_b_done = 1;

    while(1)
    {
    }
}

void task_c()
{
    for(int i = 0; i < 10; i++)
    {
        USART1_write_line("Task C is working\r\n");
    }
    while(task_a_done == 0 || task_b_done == 0);
    USART1_write_line("Task are done\r\n");
    USART1_print_number(test);
    while(1);
}

void task_d()
{
    char buff[20] = "Task D write\r\n";
    char temp[20] = "\0"; 

    for(int i = 0; i < 20; i++)
    {
        SD_write(buff, 2000 + i, strlen(buff));
    }
    
    for(int i = 0; i < 20; i++)
    {
        SD_read(temp, 2000 + i, 20);
        USART1_write_line(temp);
    }


    while(1);
}


int main()
{
    uint8_t key = 0, counter = 0;
    RCC_HSE_PLL_enable();  

    USART1_init(115200);
    USART1_interrupt_enable();
    
    SPI1_init();
    SD_begin();
    DMA1_init();
    

    sched_init();


    sched_task_create(task_d);

    sched_enable();
    
    
    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 0);
    }
}


