#ifndef SERIAL_H
#define SERIAL_H

/* Serial interface */

#include "acl.h"

void uart_init(void);

void uart_putc(char c);

int uart_getc(void);

#endif
