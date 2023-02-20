#ifndef __SOFT_I2C_MSP_H__
#define __SOFT_I2C_MSP_H__

#include "main.h"
#include "soft_i2c.h"

extern Soft_I2CObjectType soft_i2c;

void SetSCLPin(Soft_I2CPinValue op);
void SetSDAPin(Soft_I2CPinValue op);
uint8_t ReadSDAPin(void);
void delay_us(uint32_t us);

#endif /* __SOFT_I2C_MSP_H__ */
