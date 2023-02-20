#include "m_tim.h"

static uint32_t tick = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1)
    {
        tick++;
    }
}

uint32_t HAL_GetTick_us()
{
    return __HAL_TIM_GET_COUNTER(&htim1);
}

void HAL_Delay_us(uint32_t us)
{
    uint32_t tickstart = HAL_GetTick_us();
    if(us<0xFFFF)
    {
        us++;
    }
    while ((HAL_GetTick_us() - tickstart) < us)
    {
    }
}

void HAL_Delay_us_init()
{
    HAL_TIM_Base_Start_IT(&htim1);
}
