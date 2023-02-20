#include "pwm.h"

const int tim_freq = 72000000;

typedef struct pwm_t
{
    int freq;
    int duty;
} pwm_t;

static pwm_t pwm;

void pwm_init()
{
    pwm.freq = 100;
    pwm.duty = 50;

    TIM2->PSC = 10;
    TIM2->ARR = 65453;
    TIM2->CCR1 = 32726;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void pwm_freq(int freq)
{
    pwm.freq = freq;
    int tmp = tim_freq / freq;
    int arr = 0;
    int psc = 0;
    while (tmp > 65535)
    {
        psc++;
        tmp = tim_freq / freq / (psc + 1);
    }
    arr = tmp - 1;
    TIM2->PSC = psc;
    TIM2->ARR = arr;
    TIM2->CCR1 = pwm.duty*(arr+1)/100;
}

void pwm_duty(int duty)
{
    pwm.duty = duty;
    int tmp=TIM2->ARR;
    int tmp2=(tmp+1)*duty/100;
    TIM2->CCR1=tmp2;
}

void pwm_help()
{
    printf("pwm(freq=%d,duty=%d)\n", pwm.freq, pwm.duty);
    int temp=72000000/(TIM2->PSC+1)/(TIM2->ARR+1);
    int temp2=TIM2->CCR1*100/(TIM2->ARR+1);
    printf("pwm_t(freq=%d,duty=%d)\n", temp,temp2);
}
