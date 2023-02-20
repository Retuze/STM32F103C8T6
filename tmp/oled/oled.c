#include "oled.h"
#include "codetab.h"

Soft_I2CObjectType oled_i2c;
//设置SCL引脚
void oled_SetSCLPin(Soft_I2CPinValue op)
{
    if (op == Set)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    }
}

//设置SDA引脚
void oled_SetSDAPin(Soft_I2CPinValue op)
{
    if (op == Set)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
    }
}

//读取SDA引脚
uint8_t oled_ReadSDAPin(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
}

void oled_delay_us(uint32_t us)
{
    for (int i = 0; i < us; i++)
    {
        ;
    }
}

void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    uint8_t _data[2] = {addr, data};
    WriteDataBySoft_I2C(&oled_i2c, OLED_ADDRESS, _data, 2);
}

void WriteCmd(uint8_t I2C_Command) //写命令
{
    I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(uint8_t I2C_Data) //写数据
{
    I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
    HAL_Delay(100); //这里的延时很重要

    WriteCmd(0xAE); // display off
    WriteCmd(0x20); // Set Memory Addressing Mode
    WriteCmd(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0); // Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8); // Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //亮度调节 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); // 0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on oled panel
    OLED_Clear();
}

void OLED_SetPos(uint8_t x, uint8_t y)
{
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

// X:0~128,Y:0~7
void OLED_ShowStr(uint8_t x, uint8_t y, char ch[], uint8_t len, uint8_t Size)
{
    uint8_t c = 0, i = 0, j = 0;
    switch (Size)
    {
    case OLED_FONT6x8:
    {
        if (ch[j] != '\0')
        {
            while (len--)
            {
                c = ch[j] - 32;
                if (x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 6; i++)
                    WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        }
    }
    break;
    case OLED_FONT8x16:
    {
        if (ch[j] != '\0')
        {
            while (len--)
            {
                c = ch[j] - 32;
                if (x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 8; i++)
                    WriteDat(F8X16[c * 16 + i]);
                OLED_SetPos(x, y + 1);
                for (i = 0; i < 8; i++)
                    WriteDat(F8X16[c * 16 + i + 8]);
                x += 8;
                j++;
            }
        }
    }
    break;
    }
}
void OLED_P16x16Ch(unsigned char x, unsigned char y,unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		WriteCmd(0xb0+i);
		WriteCmd (0x00);
		WriteCmd (0x10);
		for(n=0;n<128;n++)
			WriteDat(0);
	}
}
