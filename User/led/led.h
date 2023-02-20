#ifndef _LED_H_
#define _LED_H_

#include "main.h"
#include "gpio.h"

void led_write(uint8_t pin, uint8_t state);
void led_toggle(uint8_t pin);
void led_init(void);

#endif // _LED_H_
