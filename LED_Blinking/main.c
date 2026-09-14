#include <stdint.h>

#define AHB1PERIPH                          (0x40020000UL)
#define GPIOD_OFFSET                        (0x0C00UL)
#define GPIOD_BASE                          (AHB1PERIPH + GPIOD_OFFSET)
#define RCC_OFFSET                          (0x3800UL)
#define RCC_BASE                            (AHB1PERIPH + RCC_OFFSET)
#define AHB1ENR_R_OFFSET                    (0x30UL)
#define RCC_AHB1ENR                         (* (volatile unsigned int *) (RCC_BASE + AHB1ENR_R_OFFSET))
#define MODER_OFFSET                        (0x00UL)
#define GPIOD_MODER                         (* (volatile unsigned int *) (GPIOD_BASE   + MODER_OFFSET))
#define ODR_OFFSET                          (0x14UL)
#define GPIOD_ODR                           (* (volatile unsigned int *) (GPIOD_BASE + ODR_OFFSET))

#define GPIOD_EN                            (1U<<3)
#define LED_PIN                             (1U<<12)


int main(){
    RCC_AHB1ENR |= GPIOD_EN;

    GPIOD_MODER |=  (1U<<24);
    GPIOD_MODER &= ~(1U<<25);

    while(1){
        GPIOD_ODR ^= LED_PIN;
        for(uint32_t i=0; i<1000000; i++){

        }
    }
}