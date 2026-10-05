#ifndef SPI_H
#define SPI_H

#include "stm32f411ve.h"
#include <stdint.h>

void spi_gpio_init(void);
void spi1_config(void);

void spi1_transmit(uint8_t *data, uint32_t size);
void spi1_receive(uint8_t *data, uint32_t size);

uint8_t spi1_transfer(uint8_t data);

void cs_enable(void);
void cs_disable(void);


#endif
