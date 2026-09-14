#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOD_EN                            (1U<<3)
#define LED_PIN                             (1U<<12)


int main(){
    RCC->AHB1ENR |= GPIOD_EN;

    GPIOD->MODER |=  (1U<<24);
    GPIOD->MODER &= ~(1U<<25);

    while(1){
        GPIOD->ODR ^= LED_PIN;
        for(uint32_t i=0; i<500000; i++){

        }
    }
}