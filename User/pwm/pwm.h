#ifndef _PWM_H_
#define _PWM_H_

#include "main.h"
#include "stdio.h"
#include "tim.h"

void pwm_freq(int freq);
void pwm_duty(int duty);
void pwm_init();
void pwm_help();

#endif // _PWM_H_
