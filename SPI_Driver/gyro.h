#ifndef I3G4250D_H
#define I3G4250D_H

#include <stdint.h>

/* I3G4250D Register Addresses */
#define I3G4250D_WHO_AM_I      0x0F

#define I3G4250D_CTRL_REG1     0x20
#define I3G4250D_CTRL_REG2     0x21
#define I3G4250D_CTRL_REG3     0x22
#define I3G4250D_CTRL_REG4     0x23
#define I3G4250D_CTRL_REG5     0x24

#define I3G4250D_OUT_X_L       0x28
#define I3G4250D_OUT_X_H       0x29

#define I3G4250D_OUT_Y_L       0x2A
#define I3G4250D_OUT_Y_H       0x2B

#define I3G4250D_OUT_Z_L       0x2C
#define I3G4250D_OUT_Z_H       0x2D

/* SPI read/write bits */
#define I3G4250D_SPI_READ      0x80
#define I3G4250D_SPI_AUTOINC   0x40

/* Expected WHO_AM_I value */
#define I3G4250D_ID            0xD3

void i3g4250d_init(void);

uint8_t i3g4250d_read_reg(uint8_t reg);
void i3g4250d_write_reg(uint8_t reg, uint8_t value);

void i3g4250d_read_xyz(int16_t *x, int16_t *y, int16_t *z);

#endif