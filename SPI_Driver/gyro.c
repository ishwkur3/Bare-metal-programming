#include <stddef.h>
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
    
    cs_enable();                         
    // FIXED: Changed from spi1_transmit to the new spi1_transfer function
    spi1_transfer(tx_buffer, NULL, 2);         
    cs_disable();                        
}


/**
 * @brief  Reads a single configuration byte from a specific sensor register.
 */
uint8_t gyro_read_register(uint8_t reg_addr)
{
    uint8_t tx_buffer[2] = { reg_addr | L3GD20_SPI_READ, 0x00 };
    uint8_t rx_buffer[2] = { 0x00, 0x00 };
    
    cs_enable();
    spi1_transfer(tx_buffer, rx_buffer, 2);
    cs_disable();
    
    return rx_buffer[1]; // The data byte returns on the second clock cycle
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
    uint8_t start_cmd = L3GD20_OUT_X_L | L3GD20_SPI_READ | L3GD20_SPI_AUTO_INC;
    
    // Allocate 7 bytes: 1 for command byte + 6 for output registers
    uint8_t tx_buffer[7] = {0};
    uint8_t rx_buffer[7] = {0};
    
    tx_buffer[0] = start_cmd;
    
    cs_enable();
    // Double check that it is lowercase "spi1_transfer", NOT "SPI1_transfer"
    spi1_transfer(tx_buffer, NULL, 2); 

    cs_disable();
    
    // Index 0 contains junk from command byte phase. Indices 1-6 contain data.
    data->x = (int16_t)((rx_buffer[2] << 8) | rx_buffer[1]);
    data->y = (int16_t)((rx_buffer[4] << 8) | rx_buffer[3]);
    data->z = (int16_t)((rx_buffer[6] << 8) | rx_buffer[5]);
}