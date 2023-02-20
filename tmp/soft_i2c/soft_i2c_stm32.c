#include "soft_i2c_stm32.h"

Soft_I2CObjectType soft_i2c;

//设置SCL引脚
void SetSCLPin(Soft_I2CPinValue op)
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
void SetSDAPin(Soft_I2CPinValue op)
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
uint8_t ReadSDAPin(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
}

void delay_us(uint32_t us)
{
    for (int i = 0; i < us; i++)
    {
        ;
    }
}


