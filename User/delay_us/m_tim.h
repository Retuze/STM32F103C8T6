#ifndef _M_TIM_H_
#define _M_TIM_H_

#include "main.h"
#include "tim.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_Delay_us(uint32_t us);
uint32_t HAL_GetTick_us();
void HAL_Delay_us_init();

#endif // _M_TIM_H_
