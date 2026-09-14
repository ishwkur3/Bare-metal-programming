#include <stdint.h>
#include "gpio.h"


int main(){
    led_init();

    while(1){
        
        led_on();
        // for(uint32_t i=0; i<100000; i++){}
        // led_off();
        // for(uint32_t i=0; i<10000000; i++){}
    }
}