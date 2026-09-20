#include "uart.h"

// Custom function to send strings over UART without standard libraries
void uart2_print_string(char *str)
{
    while (*str)
    {
        uart2_write(*str++);
    }
}

int main(void)
{
    /* 1. Initialize the UART hardware module */
    uart2_init();
    
    /* 2. Print initial test message */
    uart2_print_string("--- Isolated Bare-Metal UART Driver Running ---\r\n");
    
    while(1)
    {
        /* Endless test transmission loop */
        uart2_print_string("UART Data Stream Testing... Success!\r\n");
        
        /* Execution stall delay loop */
        for(volatile uint32_t i = 0; i < 500000; i++);
    }
}
