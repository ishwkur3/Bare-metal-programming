#ifndef __UART_H__
#define __UART_H__
#include "stm32f411ve.h"

void uart_init(void);
void uart_write(char *str);
void uart_print(const char *format, ...);
#endif