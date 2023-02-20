#ifndef _OLED_H_
#define _OLED_H_

#define OLED_ADDRESS 0x78
#define OLED_FONT6x8 0
#define OLED_FONT8x16 1

#include "main.h"
#include "soft_i2c/soft_i2c.h"

void oled_SetSCLPin(Soft_I2CPinValue op);
void oled_SetSDAPin(Soft_I2CPinValue op);
uint8_t oled_ReadSDAPin(void);
void oled_delay_us(uint32_t us);

void I2C_WriteByte(uint8_t addr, uint8_t data);
void WriteCmd(uint8_t I2C_Command);
void WriteDat(uint8_t I2C_Data);
void OLED_Init(void);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_ShowStr(uint8_t x, uint8_t y, char ch[],uint8_t len, uint8_t Size);
void OLED_P16x16Ch(unsigned char x, unsigned char y,unsigned char N);

#endif // _OLED_H_
