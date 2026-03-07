#include <stdint.h>
#include "./include/GPIO.h"
#include "./include/RCC.h"
#include "./include/USART.h"
#include "./include/SD.h"


char start[SECTOR_SIZE] = "Hello World\r\n";
char temp[SECTOR_SIZE];



void delay(uint32_t delay)
{
    while(delay)
    {
        delay--;
    }
}



int main()
{
    uint8_t key = 0, counter = 0;
    RCC_HSI_PLL_enable();
    RCC_APB2DevEnable(RCC_APB2_ENB_PORT_A);
    GPIO_pinMode(GPIO_BASE_A, 0, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    GPIO_pinMode(GPIO_BASE_A, 1, GPIO_MODE_OUTPUT_10Mhz, GPIO_CFG_OUTPUT_PUSH_PULL);
    
    USART1_init(9600);
    SPI1_init();
    SD_reset();
    SD_init();
    uint8_t res;
    USART_write_line(USART1_BASE, "testing 4\r\n");   
    while(1)
    {
        // SD_adjust_freq(6);
        // CS_high();
        
        // res = SPI_transmit_poll(SPI1_BASE, 0x55);
        // delay(1000000);

        // res = SPI_transmit_poll(SPI1_BASE, 0xaa);

        // if(res == 0xaa)
        // {
        //     USART_write_line(USART1_BASE, "SPI Works\r\n");
        //     continue;
        // }

        // USART_write_line(USART1_BASE, "SPI Failed\r\n");


        // if(res == 0xFF) USART_write_line(USART1_BASE, "HIGH\r\n");  
        // else if(res == 0x00) USART_write_line(USART1_BASE, "LOW\r\n");
        // CS_low();
        // delay(1000000);
        
        // CS_high();
        // SD_send_command(0xFF, 0xFFFFFFFF, 0xFF);
        // GPIO_pinToggle(GPIO_BASE_A, 1);
        // delay(1000000);
        // SD_send_command(0x00, 0, 0x00);
        // delay(1000000);
    }
}


