#include "SPI.h"

#define SPI1EN                          (1U<<12)
#define GPIOAEN                         (1U<<0)
#define GPIOEEN                         (1U<<4) /* Correct bit 4 for GPIOE clock gating */
#define SR_TXE                          (1U<<1)
#define SR_RXNE                         (1U<<0)
#define SR_BSY                          (1U<<7)

void spi_gpio_init(void)
{
    /* 1. Enable peripheral clock routing to GPIOA and GPIOE */
    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB1ENR |= GPIOEEN;
    
    /* Short software stall step to let the peripheral clock grids stabilize */
    volatile uint32_t delay = RCC->AHB1ENR;
    (void)delay;

    /* 2. Set PA5, PA6, PA7 mode to Alternate Function Mode (10) */
    GPIOA->MODER &= ~((3U<<10) | (3U<<12) | (3U<<14)); // Clear PA5, PA6, PA7 fields completely
    GPIOA->MODER |=  ((2U<<10) | (2U<<12) | (2U<<14)); // Apply Alternate Function Mode

    /* 3. Configure PE3 as a standard digital Output Pin (01) for Chip Select */
    GPIOE->MODER &= ~(3U<<6);  // Clear both configuration bits 6 and 7
    GPIOE->MODER |=  (1U<<6);  // Set bit 6 to 1 (Output Mode: 01)
    
    /* Force Chip Select line to begin in an inactive (HIGH) state */
    GPIOE->ODR |= (1U<<3);

    /* 4. Link PA5, PA6, PA7 explicitly to SPI1 Alternate Function (AF5 = 0101) */
    GPIOA->AFRL &= ~((0xFU<<20) | (0xFU<<24) | (0xFU<<28)); // Wipe current mux configs
    
    GPIOA->AFRL |=  (5U<<20);  // PA5 -> SPI1_SCK
    GPIOA->AFRL |=  (5U<<24);  // PA6 -> SPI1_MISO
    GPIOA->AFRL |=  (5U<<28);  // PA7 -> SPI1_MOSI
}

void spi1_config(void)
{
    /* Enable clock access to SPI1 module */
    RCC->APB2ENR |= SPI1EN;

    /* Set clock baud rate to fPCLK/4 (Bit 3 = 1, Bits 4:5 = 0) */
    SPI1->SPI_CR1 |= (1U<<3);
    SPI1->SPI_CR1 &= ~(1U<<4);
    SPI1->SPI_CR1 &= ~(1U<<5);

    /* FIXED: Set to SPI Mode 0 (CPOL=0, CPHA=0) for absolute initialization stability */
    SPI1->SPI_CR1 &= ~(1U<<0); // CPHA = 0
    SPI1->SPI_CR1 &= ~(1U<<1); // CPOL = 0

    /* Enable full duplex operations */
    SPI1->SPI_CR1 &= ~(1U<<10);

    /* Set transmission layout format bit arrangement to MSB first */
    SPI1->SPI_CR1 &= ~(1U<<7);

    /* Configure hardware module profile role as MASTER */
    SPI1->SPI_CR1 |= (1U<<2);

    /* Set data transfer framing resolution to 8-Bit Data Mode */
    SPI1->SPI_CR1 &= ~(1U<<11);

    /* Select software slave management variables (SSM=1 and SSI=1) to prevent MODF faults */
    SPI1->SPI_CR1 |= (1U<<8);
    SPI1->SPI_CR1 |= (1U<<9);

    /* Enable the SPI hardware engine module */
    SPI1->SPI_CR1 |= (1U<<6);
}

/**
 * @brief Sends and receives data simultaneously over SPI1 (Full Duplex)
 */
void spi1_transfer(uint8_t *tx_data, uint8_t *rx_data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        // Wait until transmit buffer is empty
        while (!(SPI1->SPI_SR & (1U << 1))) {}
        
        // Write byte to trigger clocks
        SPI1->SPI_DR = tx_data ? tx_data[i] : 0x00;
        
        // Wait until receive buffer is full
        while (!(SPI1->SPI_SR & (1U << 0))) {}
        
        // Read byte safely
        uint8_t dummy = SPI1->SPI_DR;
        if (rx_data)
        {
            rx_data[i] = dummy;
        }
    }
    // Wait until SPI is completely idle
    while (SPI1->SPI_SR & (1U << 7)) {}
}


void spi1_receive(uint8_t *data, uint32_t size)
{
    while(size)
    {
        /* Confirm transmission register paths are clear before pulsing clocks */
        while(!(SPI1->SPI_SR & (SR_TXE))){}
        
        /* Force master clock generations by dropping dummy byte inside register */
        SPI1->SPI_DR = 0;
        
        /* Wait until the hardware flag Receive Buffer Not Empty (RXNE) drops true */
        while(!(SPI1->SPI_SR & (SR_RXNE))){}
        
        /* Extract data response byte out from register target location pointer */
        *data++ = (SPI1->SPI_DR);
        size--;
    }
}

void cs_enable(void)
{
    GPIOE->ODR &= ~(1U<<3); // Drive PE3 LOW to select gyro
}

void cs_disable(void)
{
    GPIOE->ODR |= (1U<<3);  // Drive PE3 HIGH to return gyro to idle
}
