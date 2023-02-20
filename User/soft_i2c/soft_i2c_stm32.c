#include "soft_i2c_stm32.h"

//设置SCL引脚
void SetSCLPin(Soft_I2CPinValue op)
{
    if (op == IIC_SET)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    }
}

//设置SDA引脚
void SetSDAPin(Soft_I2CPinValue op)
{
    if (op == IIC_SET)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
    }
}

//读取SDA引脚
uint8_t ReadSDAPin(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
}



