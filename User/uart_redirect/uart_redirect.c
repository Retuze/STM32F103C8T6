#include "uart_redirect.h"
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART3 and Loop until the end of transmission */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
int _write(int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        __io_putchar(*pBuffer++);
    }
    return size;
}
