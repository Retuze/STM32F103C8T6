#ifndef __SOFT_I2C_MSP_H__
#define __SOFT_I2C_MSP_H__

#include "main.h"
#include "soft_i2c.h"


void SetSCLPin(Soft_I2CPinValue op);
void SetSDAPin(Soft_I2CPinValue op);
uint8_t ReadSDAPin(void);
#endif /* __SOFT_I2C_MSP_H__ */
