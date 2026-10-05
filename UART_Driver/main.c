#include "uart.h"

// Custom function to send strings over UART without standard libraries

int main(void)
{
    /* 1. Initialize the UART hardware module */
    uart2_init();
    
    while(1)
    {
        /* Endless test transmission loop */
        uart2_write("UART Data Stream Success...\r\n");
        
        /* Execution stall delay loop */
        for(volatile uint32_t i = 0; i < 500000; i++)
        {
            uart_print("Counter i = %d\r\n",i);
        }
    }
}
