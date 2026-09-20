#include "gyro.h"
#include "SPI.h"  // Pulls in your spi1_transmit, spi1_receive, and cs operations

/**
 * @brief  Writes a single byte of configuration data to a sensor register.
 */
void gyro_write_register(uint8_t reg_addr, uint8_t value)
{
    uint8_t tx_buffer[2];
    
    // Format command frame: [Address byte (Write Mode)] -> [Data byte]
    tx_buffer[0] = reg_addr; 
    tx_buffer[1] = value;
    
    cs_enable();                         // Pull PE3 Low
    spi1_transmit(tx_buffer, 2);         // Stream out packet
    cs_disable();                        // Release PE3 High
}

/**
 * @brief  Reads a single configuration byte from a specific sensor register.
 */
uint8_t gyro_read_register(uint8_t reg_addr)
{
    uint8_t command_byte = reg_addr | L3GD20_SPI_READ;
    uint8_t received_val = 0;
    
    cs_enable();                         // Pull PE3 Low
    spi1_transmit(&command_byte, 1);     // Request specific register address
    spi1_receive(&received_val, 1);      // Clock out response value
    cs_disable();                        // Release PE3 High
    
    return received_val;
}

/**
 * @brief  Initializes the gyroscope sensor over the SPI bus.
 * @retval 1 if communication succeeded, 0 if device wasn't detected.
 */
uint8_t gyro_init(void)
{
    // 1. Verify connection by fetching hardware identifier
    uint8_t chip_id = gyro_read_register(L3GD20_WHO_AM_I);
    if (chip_id != 0xD4)
    {
        return 0; // Device not found or SPI transmission broken
    }
    
    // 2. Configure CTRL_REG1 (0x20):
    // Bits [7:6] = 00 -> Data Rate 95Hz
    // Bits [5:4] = 00 -> Bandwidth 12.5Hz
    // Bit 3      = 1  -> Normal power mode (Wake up from sleep)
    // Bits [2:0] = 111-> Enable Z, Y, X axes
    gyro_write_register(L3GD20_CTRL_REG1, 0x0FU);
    
    // 3. Configure CTRL_REG4 (0x23):
    // Bit 7      = 1  -> Block Data Update (Prevents MSB/LSB updates mid-read)
    // Bits [5:4] = 00 -> Full scale selection: 250 dps (Degrees Per Second)
    gyro_write_register(L3GD20_CTRL_REG4, 0x80U);
    
    return 1; // Initialization successful
}

/**
 * @brief  Reads raw 3-axis angular velocity measurements in a single burst step.
 */
void gyro_read_data(Gyro_Data_t *data)
{
    // Start address with READ bit and AUTO_INC bit active
    uint8_t start_cmd = L3GD20_OUT_X_L | L3GD20_SPI_READ | L3GD20_SPI_AUTO_INC;
    uint8_t raw_bytes[6];
    
    cs_enable();                                // Pull PE3 Low
    spi1_transmit(&start_cmd, 1);               // Send multi-byte read target
    spi1_receive(raw_bytes, 6);                 // Fetch all 6 axis data bytes sequentially
    cs_disable();                               // Release PE3 High
    
    // Assemble high and low 8-bit bytes into standard 16-bit signed variables
    data->x = (int16_t)((raw_bytes[1] << 8) | raw_bytes[0]);
    data->y = (int16_t)((raw_bytes[3] << 8) | raw_bytes[2]);
    data->z = (int16_t)((raw_bytes[5] << 8) | raw_bytes[4]);
}
