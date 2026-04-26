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

lock_t lock = 0;
uint32_t test = 0;
uint32_t task_a_done = 0;
uint32_t task_b_done = 0;

uint8_t block_buff[512];

void delay(uint32_t delay)
{
    while(delay--);
}

void increment()
{
    test++;
    delay(100);
}

void task_a()
{
    
    for(int i = 0; i < 10000; i++)
    {
        increment();
    }

    task_a_done = 1;
        
    while(1);
}

void task_b()
{
    
    for(int i = 0; i < 10000; i++)
    {
        increment();
    }

    task_b_done = 1;

    while(1);
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
    char buff[20] = "Task D write t 2\r\n";
    char temp[20] = "\0"; 

    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_50Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinToggle(GPIO_BASE_A, 0);

    for(int i = 0; i < 200; i++)
    {
        SD_write(buff, 2000 + i, strlen(buff));
    }
    
    for(int i = 0; i < 200; i++)
    {
        SD_read(temp, 2000 + i, 20);
        USART1_print_number(i);
        USART1_write_line(temp);
    }


    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 0);
        delay(100000);
    }
}


void task_d()
{
    char buff[20] = "Task D write t 2\r\n";
    char temp[20] = "\0"; 

    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_50Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinToggle(GPIO_BASE_A, 0);

    for(int i = 0; i < 200; i++)
    {
        SD_write(buff, 2000 + i, strlen(buff));
    }
    
    for(int i = 0; i < 200; i++)
    {
        SD_read(temp, 2000 + i, 20);
        USART1_print_number(i);
        USART1_write_line(temp);
    }

    while(1);
}

void task_e()
{
    int i = 0;
    char buff[20] = "Task E write\r\n";
    char temp[20] = "\0"; 
    while(1)
    {
        memset(temp, 0, 20);
        SD_write(buff, 3000 + (i % 200), strlen(buff));
    
        SD_read(temp, 3000 + (i % 200), 20);
        USART1_print_number(i);
        USART1_write_line(temp);
        
        i++;
        
    }
}


void display_image()
{
    uint32_t size = 64800;
    uint32_t start_off = 5000;
    uint32_t bytes_to_read;
    for(int i = 0; i < 4; i++)
    {
        while(size > 0)
        {
            bytes_to_read = size > 512 ? 512 : size;
            size -= SD_read(block_buff, start_off, bytes_to_read);
            for(int j = 0; j < bytes_to_read; j++)
            {
                LCD_data(block_buff[j]);
            }
            
            if(size == 0) break;
            start_off++;
        }
        LCD_set_window(0, 0, SCREEN_W - 1, SCREEN_H - 1);
        size = 64800 - bytes_to_read;   
        
        for(int j = 0; j < 512 - bytes_to_read; j++)
        {
            LCD_data(block_buff[j]);
        }
        start_off++;

        
    }
}

void task_f()
{
    char buff[20] = "Task F testing\r\n";
    uint32_t blocks = 507;
    
    LCD_set_window(0, 0, SCREEN_W - 1, SCREEN_H - 1);


    while(1)
    {
        display_image();
    }

}


int main()
{
    uint8_t key = 0, counter = 0;
    RCC_HSE_PLL_enable();  

    USART1_init(115200);
    USART1_interrupt_enable();
    
    SPI1_init();
    SPI2_init();
    LCD_init();
    SD_begin();
    DMA1_init();
    
    

    sched_init();

    sched_task_create(task_a);
    sched_task_create(task_b);
    sched_task_create(task_c);
    sched_task_create(task_e);
    sched_task_create(task_d);
    sched_task_create(task_f);

    sched_enable();
    
    
    while(1)
    {
        GPIO_pinToggle(GPIO_BASE_A, 0);
    }
}


