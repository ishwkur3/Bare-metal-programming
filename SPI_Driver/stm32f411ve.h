#ifndef STM32F411VE_H
#define STM32F411VE_H
#include <stdint.h>

typedef struct{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
}GPIO_TypeDef;

typedef struct{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t RESERVED3[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t RESERVED4[2];
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t RESERVED5[2];
    volatile uint32_t BDCRR;
    volatile uint32_t CSR;
    volatile uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t RESERVED7[1];
    volatile uint32_t DCKCFGR;
}RCC_TypeDef;

typedef struct{
    volatile uint32_t SPI_CR1;
    volatile uint32_t SPI_CR2;
    volatile uint32_t SPI_SR;
    volatile uint32_t SPI_DR;
    volatile uint32_t SPI_CRCPR;
    volatile uint32_t SPI_RXCRCR;
    volatile uint32_t SPI_TXCRCR;
    volatile uint32_t SPI_I2SCFGR;
    volatile uint32_t SPI_I2SPR;
}SPI1_TypeDef;

#endif

#define PERIPH_BASE                                 (0X40000000UL)
#define AHB1PERIPH_BASE                             (PERIPH_BASE + 0x00020000UL)
#define APB2PERIPH_BASE                             (PERIPH_BASE + 0x00010000UL)

#define GPIOA_BASE                                  (AHB1PERIPH_BASE)
#define GPIOD_BASE                                  (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE                                  (AHB1PERIPH_BASE + 0x1000UL)
#define RCC_BASE                                    (AHB1PERIPH_BASE + 0x3800UL)

#define GPIOA                                       ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOD                                       ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE                                       ((GPIO_TypeDef *) GPIOE_BASE)
#define RCC                                         ((RCC_TypeDef *)  RCC_BASE)


#define SPI1_BASE                                   (APB2PERIPH_BASE + 0x3000UL)
#define SPI1                                        ((SPI1_TypeDef *) SPI1_BASE)
