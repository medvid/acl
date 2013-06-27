/* Parts taken from KEIL ARM development tools libraries */

#include "lpc213x.h"                    /* LPC213x definitions */
#include "system.h"
#include "acl.h"

#define CR 0x0D                         /* carriagereturn character */
#define BAUD_RATE 9600
#define WIDTH 80                        /* max chars in line */

int no_chars = 0;

void init_serial(void) {                /* Initialize Serial Interface */
    int tmp;
    PINSEL0 = 0x00050000;               /* Enable RxD1 and TxD1 */
    U1LCR = 0x83;                       /* 8 bits, no Parity, 1 Stop bit */
    tmp = FREQUENCY/BAUD_RATE;
    tmp *= (PLLSTAT & 0x1F) + 1;
    if((VPBDIV & 3) == 0) tmp >>= 2;
    if((VPBDIV & 3) == 2) tmp >>= 1;
    tmp >>= 4;
    U1DLL = tmp & 0xFF;
    U1DLM = (tmp >> 8) & 0xFF;
    U1LCR = 0x03;                       /* DLAB = 0*/
}

void put_char(int ch) {                 /* Write character to Serial Port */
    if((no_chars == WIDTH) || (ch == '\n')) {
        no_chars = 0;
        while(!(U1LSR & 0x20));
        U1THR = CR;
        while(!(U1LSR & 0x20));
        U1THR = '\n';
    } else {
        no_chars++;
        while(!(U1LSR & 0x20));
        U1THR = ch;
    }
}

int get_char(void) {                    /* Read character from Serial Port */
    while(!(U1LSR & 0x01));
    return(U1RBR);
}

void put_hex(unsigned int hex) {        /* Write Hex Digit to Serial Port */
    int tmp;

    tmp = hex - 10;
    if(tmp >= 0) put_char('A' + tmp); else put_char('0' + hex);
}

void put_int(unsigned int x) {
    int i;
    for(i=0; i<8; i++) {
        put_hex((x >> 28) & 0x0F);
        x <<= 4;
    }
}

void put_vect(unsigned int *p, unsigned int len) {
    uint i;

    for(i=len; i; i--) put_int(p[i-1]);
}

void put_str(char *p) {                 /* Write string */
    while(*p) put_char(*p++);
}

char data[11];

void put_val(char *p, uint value) {
    put_str(p);
    acl_hex2str_dec(data, 10, &value, 1);
    data[10] = '\0';
    put_str(data);
}
