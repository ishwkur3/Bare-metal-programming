#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "uart.h"

#define GPIOAEN             (1U<<0)
#define UART2EN             (1U<<17)

#define DBG_UART_BAUDRATE   115200
#define SYS_FREQ            32000000
#define APB1_CLK            SYS_FREQ
#define CR1_TE              (1U<<3)
#define CR1_UE              (1U<<13)
#define SR_TXE              (1U<<7)

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
void uart2_write(char *str);
void uart_print(const char *format, ...);

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

void uart2_write(char *str)
{
    while (*str)
    {
        /* Wait until hardware Transmit Data Register Empty (TXE) flag is active */
        while (!(UART2->SR & SR_TXE)) {}
        
        /* Write byte directly to transmission data register buffer */
        UART2->DR = (*str & 0xFF);
        *str++;
    }
}

void uart_write_char(char c)
{
    while (!(UART2->SR & SR_TXE))
    {
    }

    UART2->DR = c;
}

static void uart_write_int(int32_t value)
{
    char buffer[12];
    int i = 0;

    if (value == 0)
    {
        uart_write_char('0');
        return;
    }

    if (value < 0)
    {
        uart_write_char('-');
        value = -value;
    }

    while (value > 0)
    {
        buffer[i++] = (value % 10) + '0';
        value = value / 10;
    }

    while (i > 0)
    {
        uart_write_char(buffer[--i]);
    }
}

void uart_print(const char *format, ...)
{
    va_list args;

    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;

            switch (*format)
            {
                case 'd':
                {
                    int32_t value = va_arg(args, int32_t);
                    uart_write_int(value);
                    break;
                }

                case 'c':
                {
                    char value = (char)va_arg(args, int);
                    uart_write_char(value);
                    break;
                }

                case 's':
                {
                    char *value = va_arg(args, char *);
                    uart2_write(value);
                    break;
                }

                case '%':
                {
                    uart_write_char('%');
                    break;
                }

                default:
                {
                    uart_write_char('%');
                    uart_write_char(*format);
                    break;
                }
            }
        }
        else
        {
            uart_write_char(*format);
        }

        format++;
    }

    va_end(args);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
    return ((periph_clk + baudrate/2U)/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
    UART2->BRR = compute_uart_bd(periph_clk, baudrate);
}
