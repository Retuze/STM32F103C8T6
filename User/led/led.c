#include "led.h"

void led_write(uint8_t pin, uint8_t state)
{
    if (pin > 3 || pin == 0)
    {
        return;
    }
    if(state>1)
    {
        return;
    }
    switch (pin)
    {
    case 1:
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, state);
        break;
    case 2:
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, state);
        break;
    case 3:
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, state);
        break;
    default:
        break;
    }
}
void led_toggle(uint8_t pin)
{
        if (pin > 3 || pin == 0)
    {
        return;
    }
    switch (pin)
    {
    case 1:
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        break;
    case 2:
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        break;
    case 3:
        HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        break;
    default:
        break;
    }
}
void led_init()
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
}
