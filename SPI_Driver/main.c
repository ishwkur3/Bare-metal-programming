#include "uart.h"
#include "SPI.h"
#include "gyro.h"

int main(void)
{
    int16_t x;
    int16_t y;
    int16_t z;

    /* Initialize UART */
    uart_init();

    /* Initialize SPI GPIO */
    spi_gpio_init();

    /* Configure SPI1 */
    spi1_config();

    /* Initialize I3G4250D */
    i3g4250d_init();

    while (1)
    {
        /* Read gyro */
        i3g4250d_read_xyz(&x, &y, &z);

        /* Print raw gyro values */
        uart_print("(X,Y,Z):(%d,%d,%d)\r\n", x, y, z);

        /* Delay */
        for (volatile uint32_t i = 0; i < 500000; i++)
        {
        }
    }
}