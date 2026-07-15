#include <stdio.h> 
#include <stdint.h>

#include <devices/uart.h>

void uart_tx(const uint32_t data)
{
    putchar(data);
}