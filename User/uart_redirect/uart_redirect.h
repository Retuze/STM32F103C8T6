#ifndef _UART_REDIRECT_H_
#define _UART_REDIRECT_H_

#include "usart.h"
#include "stdio.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE;
int _write(int fd, char *pBuffer, int size);

/*****************不使用MicroLIB库需加上该部分****************************/
// 取消ARM的半主机工作模式
// #pragma import(__use_no_semihosting)  // 确保没有从C库链接使用半主机的函数

// struct __FILE // 标准库需要的支持函数
// {
// int handle;
// };

// FILE __stdout;        // FILE is typedef'd in stdio.h

// void _sys_exit(int x) // 定义_sys_exit()以避免使用半主机模式
// {
// x = x;
// }
/**********************************************************************/
#endif
