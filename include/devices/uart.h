#ifndef DEVICES_UART_H
#define DEVICES_UART_H

#include <stdio.h> 
#include <stdint.h>

static inline void uart_tx(const uint32_t data)
{
    putchar(data);
}

#endif 