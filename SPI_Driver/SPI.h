#ifndef SPI_H
#define SPI_H

#include "stm32f411ve.h"
#include <stdint.h>

void spi_gpio_init(void);
void spi1_config(void);

// Replace spi1_transmit and spi1_receive with this unified function:
void spi1_transfer(uint8_t *tx_data, uint8_t *rx_data, uint32_t size);

void cs_enable(void);
void cs_disable(void);

#endif
