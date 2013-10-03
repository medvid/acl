#include "stdio.h"
#include "serial.h"

void init_serial()
{
    uart_init();
}

/* Write character to Serial Port */
void put_char(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }

    uart_putc(c);
}

/* Read character from Serial Port */
int get_char(void)
{
    return uart_getc();
}

/* Write Hex Digit to Serial Port */
void put_hex(unsigned int hex)
{
    int tmp;

    tmp = hex - 10;
    if (tmp >= 0) {
        put_char('A' + tmp);
    } else {
        put_char('0' + hex);
    }
}

void put_int(unsigned int x)
{
    int i;
    for (i=0; i<8; i++) {
        put_hex((x >> 28) & 0x0F);
        x <<= 4;
    }
}

void put_vect(unsigned int *p, unsigned int len)
{
    uint i;

    for (i = len; i; i--) {
        put_int(p[i-1]);
    }
}

/* Write string */
void put_str(char *p)
{
    while (*p) {
        put_char(*p++);
    }
}

char data[11];

void put_val(char *p, uint value)
{
    put_str(p);
    acl_hex2str_dec(data, 10, &value, 1);
    data[10] = '\0';
    put_str(data);
}
