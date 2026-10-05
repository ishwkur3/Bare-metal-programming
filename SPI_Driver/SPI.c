#include "SPI.h"
#include "uart.h"

#define SPI1EN                          (1U<<12)
#define GPIOAEN                         (1U<<0)
#define GPIOEEN                         (1U<<4) /* Correct bit 4 for GPIOE clock gating */
#define SR_TXE                          (1U<<1)
#define SR_RXNE                         (1U<<0)
#define SR_BSY                          (1U<<7)

void spi_gpio_init(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB1ENR |= GPIOEEN;

    /*Set PA5,PA6,PA7 mode to alternate function*/
    /*PA5*/
    GPIOA->MODER &=~(1U<<10);
    GPIOA->MODER |=(1U<<11);
    /*PA6*/
    GPIOA->MODER &=~(1U<<12);
    GPIOA->MODER |=(1U<<13);
    /*PA7*/
    GPIOA->MODER &=~(1U<<14);
    GPIOA->MODER |=(1U<<15);
    /*Set PA9 as output pin*/
    GPIOA->MODER |=(1U<<18);
    GPIOA->MODER &=~(1U<<19);
    /*Set PA5,PA6,PA7 alternate function type to SPI1*/
    /*PA5*/
    GPIOA->AFRL |=(1U<<20);
    GPIOA->AFRL &= ~(1U<<21);
    GPIOA->AFRL |=(1U<<22);
    GPIOA->AFRL &= ~(1U<<23);
    /*PA6*/
    GPIOA->AFRL |=(1U<<24);
    GPIOA->AFRL &= ~(1U<<25);
    GPIOA->AFRL |=(1U<<26);
    GPIOA->AFRL &= ~(1U<<27);
    /*PA7*/
    GPIOA->AFRL |=(1U<<28);
    GPIOA->AFRL &= ~(1U<<29);
    GPIOA->AFRL |=(1U<<30);
    GPIOA->AFRL &= ~(1U<<31);

    /* Configure PE3 as GPIO output */
    GPIOE->MODER &= ~(3U << 6);
    GPIOE->MODER |=  (1U << 6);

    /* Set CS HIGH initially */
    GPIOE->ODR |= (1U << 3);

    uart_write("SPI init >> Sucess.\r\n");
}

void spi1_config(void)
{
    /* Enable clock access to SPI1 module */
    RCC->APB2ENR |= SPI1EN;

    /* Set clock baud rate to fPCLK/4 (Bit 3 = 1, Bits 4:5 = 0) */
    SPI1->CR1 |= (1U<<3);
    SPI1->CR1 &= ~(1U<<4);
    SPI1->CR1 &= ~(1U<<5);

    /* FIXED: Set to SPI Mode 0 (CPOL=0, CPHA=0) for absolute initialization stability */
    SPI1->CR1 &= ~(1U<<0); // CPHA = 0
    SPI1->CR1 &= ~(1U<<1); // CPOL = 0

    /* Enable full duplex operations */
    SPI1->CR1 &= ~(1U<<10);

    /* Set transmission layout format bit arrangement to MSB first */
    SPI1->CR1 &= ~(1U<<7);

    /* Configure hardware module profile role as MASTER */
    SPI1->CR1 |= (1U<<2);

    /* Set data transfer framing resolution to 8-Bit Data Mode */
    SPI1->CR1 &= ~(1U<<11);

    /* Select software slave management variables (SSM=1 and SSI=1) to prevent MODF faults */
    SPI1->CR1 |= (1U<<8);
    SPI1->CR1 |= (1U<<9);

    /* Enable the SPI hardware engine module */
    SPI1->CR1 |= (1U<<6);

    uart_write("SPI config >> Done.\r\n");
}

uint8_t spi1_transfer(uint8_t data)
{
    uint8_t received;

    /* Wait until transmit buffer is empty */
    while (!(SPI1->SR & SR_TXE))
    {
    }

    /*
     * Write one byte to SPI data register.
     */
    *((volatile uint8_t *)&SPI1->DR) = data;

    /* Wait until received data is available */
    while (!(SPI1->SR & SR_RXNE))
    {
    }

    /*
     * Read received byte
     */
    received = *((volatile uint8_t *)&SPI1->DR);

    return received;
}

void spi1_transmit(uint8_t *data,uint32_t size)
{
	uint32_t i=0;
	uint8_t temp;
	while(i<size)
	{
        /*Wait until TXE is set*/
        while(!(SPI1->SR & (SR_TXE))){}

        /*Write the data to the data register*/
        SPI1->DR = data[i];
        i++;
	}
	/*Wait until TXE is set*/
	while(!(SPI1->SR & (SR_TXE))){}
	
	/*Wait for BUSY flag to reset*/
	while((SPI1->SR & (SR_BSY))){}
	
	/*Clear OVR flag*/
	temp = SPI1->DR;
	temp = SPI1->SR;

    uart_write("SPI1 transmit.");
}

void spi1_receive(uint8_t *data, uint32_t size)
{
    while(size)
    {
        /* Confirm transmission register paths are clear before pulsing clocks */
        while(!(SPI1->SR & (SR_TXE))){}
        
        /* Force master clock generations by dropping dummy byte inside register */
        SPI1->DR = 0;
        
        /* Wait until the hardware flag Receive Buffer Not Empty (RXNE) drops true */
        while(!(SPI1->SR & (SR_RXNE))){}
        
        /* Extract data response byte out from register target location pointer */
        *data++ = (SPI1->DR);
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
