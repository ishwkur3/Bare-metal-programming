#include "SPI.h"
#include "gyro.h"
#include "GPIO.h" // For your working LED blink drivers

int main(void)
{
    // 1. Initialise micro peripherals
    led_init();         // Set up onboard LEDs
    spi_gpio_init();    // Set up SPI pins (PA5, PA6, PA7, PE3)
    spi1_config();      // Turn on internal SPI1 core
    
    // 2. Initialise Gyroscope
    if (!gyro_init())
    {
        // Blink indicator rapidly if gyro communication is dead
        while(1)
        {
            led_on();
            for(volatile uint32_t i=0; i<100000; i++);
            led_off();
            for(volatile uint32_t i=0; i<100000; i++);
        }
    }
    
    Gyro_Data_t raw_motion;
    
    while(1)
    {
        // Read updated spatial motion vectors
        gyro_read_data(&raw_motion);
        
        // --- Project Application Rule ---
        // If the board is tipped hard on its X-axis, turn the LED on!
        if (raw_motion.x > 3000 || raw_motion.x < -3000)
        {
            led_on();
        }
        else
        {
            led_off();
        }
        
        // Simple sample interval delay
        for(volatile uint32_t i=0; i<50000; i++);
    }
}
