#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "uart_redirect/uart_redirect.h"
#include "fifo/fifo.h"
#include "led/led.h"
#include "pwm/pwm.h"


#define ARRAY_SIZE(arr) sizeof(arr) / sizeof((arr)[0])

void shell_proc();
char split_string(char *str, const char *delim, char *sub_ptr[], char size);
#endif // _CONSOLE_H_
