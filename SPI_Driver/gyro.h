#ifndef GYRO_H
#define GYRO_H

#include <stdint.h>

// --- L3GD20 Internal Register Map ---
#define L3GD20_WHO_AM_I         0x0F  // Device ID Register (Should return 0xD4)
#define L3GD20_CTRL_REG1        0x20  // Power mode, Data rate, Axis enable
#define L3GD20_CTRL_REG4        0x23  // Full-scale selection, Block data update
#define L3GD20_OUT_X_L          0x28  // Output Data registers
#define L3GD20_OUT_X_H          0x29
#define L3GD20_OUT_Y_L          0x2A
#define L3GD20_OUT_Y_H          0x2B
#define L3GD20_OUT_Z_L          0x2C
#define L3GD20_OUT_Z_H          0x2D

// --- SPI Communication Protocol Flags ---
#define L3GD20_SPI_READ         (1U << 7) // Bit 7: Read (1) or Write (0) flag
#define L3GD20_SPI_AUTO_INC     (1U << 6) // Bit 6: Address Auto-Increment flag

// --- Data Structure for Gyro Readings ---
typedef struct {
    int16_t x; // Raw X-axis angular velocity
    int16_t y; // Raw Y-axis angular velocity
    int16_t z; // Raw Z-axis angular velocity
} Gyro_Data_t;

// --- Public Function Prototypes ---
uint8_t gyro_init(void);
void gyro_read_data(Gyro_Data_t *data);

#endif
