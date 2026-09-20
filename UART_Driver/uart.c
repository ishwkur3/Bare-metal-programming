#include <stdint.h>
#include "uart.h"

#define GPIOAEN             (1U<<0)
#define UART2EN             (1U<<17)

#define DBG_UART_BAUDRATE   115200
#define SYS_FREQ            16000000
#define APB1_CLK            SYS_FREQ
#define CR1_TE              (1U<<3)
#define CR1_UE              (1U<<13)
#define SR_TXE              (1U<<7)

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
void uart2_write(int ch);

int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}

void uart2_init(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOAEN;

    /*Set the mode of PA2 to alternate function mode*/
    GPIOA->MODER &= ~(1U<<4);
    GPIOA->MODER |= (1U<<5);

    /*Set alternate function type to AF7(UART2_TX)*/
    GPIOA->AFRL |= (1U<<8);
    GPIOA->AFRL |= (1U<<9);
    GPIOA->AFRL |= (1U<<10);
    GPIOA->AFRL &= ~(1U<<11);

    /*Enable clock access to UART2*/
    RCC->APB1ENR |= UART2EN;

    /*Configure uart baudrate*/
    uart_set_baudrate(APB1_CLK, DBG_UART_BAUDRATE);
    
    /*Configure transfer direction*/
    UART2->CR1 = CR1_TE;

    /*Enable UART Module*/
    UART2->CR1 |= CR1_UE;
}

// Ensure it is completely lowercase "uart2_write" and takes an "int ch"
void uart2_write(int ch)
{
    /* Wait until hardware Transmit Data Register Empty (TXE) flag is active */
    while (!(UART2->SR & (1U << 7))) {}
    
    /* Write byte directly to transmission data register buffer */
    UART2->DR = (ch & 0xFF);
}


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
    return ((periph_clk + baudrate/2U)/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
    UART2->BRR = compute_uart_bd(periph_clk, baudrate);
}
