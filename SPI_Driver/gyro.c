#include "gyro.h"
#include "SPI.h"
#include "uart.h"

/*
 * Read one register from I3G4250D
 */
uint8_t i3g4250d_read_reg(uint8_t reg)
{
    uint8_t data;

    /* Select sensor */
    cs_enable();

    /*
     * Bit 7 = 1 -> Read operation
     */
    spi1_transfer(reg | I3G4250D_SPI_READ);

    /*
     * Send dummy byte to generate clock
     * and receive sensor data
     */
    data = spi1_transfer(0x00);

    /* Deselect sensor */
    cs_disable();

    return data;
}


/*
 * Write one register to I3G4250D
 */
void i3g4250d_write_reg(uint8_t reg, uint8_t value)
{
    /* Select sensor */
    cs_enable();

    /*
     * Bit 7 = 0 -> Write operation
     */
    spi1_transfer(reg & 0x7F);

    /* Send register value */
    spi1_transfer(value);

    /* Deselect sensor */
    cs_disable();
}


/*
 * Initialize I3G4250D
 */
void i3g4250d_init(void)
{
    uint8_t who_am_i;

    /*
     * Read sensor identification register
     */
    who_am_i = i3g4250d_read_reg(I3G4250D_WHO_AM_I);

    uart_print("I3G4250D WHO_AM_I = 0x%02X\r\n", who_am_i);

    /*
     * Check sensor ID
     */
    if (who_am_i != I3G4250D_ID)
    {
        uart_write("I3G4250D NOT DETECTED!\r\n");
        return;
    }

    uart_write("I3G4250D detected successfully.\r\n");


    /*
     * CTRL_REG1 = 0x0F
     *
     * PD  = 1 -> Normal mode
     * XEN = 1 -> X-axis enabled
     * YEN = 1 -> Y-axis enabled
     * ZEN = 1 -> Z-axis enabled
     *
     * DR/BW = default configuration
     */
    i3g4250d_write_reg(I3G4250D_CTRL_REG1, 0x0F);


    /*
     * CTRL_REG4
     *
     * FS bits select full-scale range.
     *
     * 00 -> ±245 dps
     *
     * Default value 0x00 gives ±245 dps.
     */
    i3g4250d_write_reg(I3G4250D_CTRL_REG4, 0x00);

    uart_write("I3G4250D configuration complete.\r\n");
}


/*
 * Read X, Y and Z axis data
 */
void i3g4250d_read_xyz(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t buffer[6];

    /*
     * Select sensor
     */
    cs_enable();

    /*
     * Read operation + auto increment
     *
     * Starting register:
     * OUT_X_L = 0x28
     *
     * 0x80 -> read
     * 0x40 -> auto increment
     *
     * Command = 0xE8
     */
    spi1_transfer(I3G4250D_OUT_X_L |
                  I3G4250D_SPI_READ |
                  I3G4250D_SPI_AUTOINC);

    /*
     * Read six consecutive bytes:
     *
     * buffer[0] = X Low
     * buffer[1] = X High
     * buffer[2] = Y Low
     * buffer[3] = Y High
     * buffer[4] = Z Low
     * buffer[5] = Z High
     */
    for (uint8_t i = 0; i < 6; i++)
    {
        buffer[i] = spi1_transfer(0x00);
    }

    /*
     * Deselect sensor
     */
    cs_disable();

    /*
     * Combine low and high bytes
     */
    *x = (int16_t)((buffer[1] << 8) | buffer[0]);

    *y = (int16_t)((buffer[3] << 8) | buffer[2]);

    *z = (int16_t)((buffer[5] << 8) | buffer[4]);
}