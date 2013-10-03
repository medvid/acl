/* Parts taken from KEIL ARM development tools libraries */

#include "serial.h"
#include "board.h"

void uart_init(void) {                  /* Initialize Serial Interface */
    int tmp;

    PINSEL0 = 0x00050000;               /* Enable RxD1 and TxD1 */
    U1LCR = 0x83;                       /* 8 bits, no Parity, 1 Stop bit */
    tmp = CONFIG_FREQUENCY/CONFIG_BAUDRATE;
    tmp *= (PLLSTAT & 0x1F) + 1;
    if ((VPBDIV & 3) == 0) tmp >>= 2;
    if ((VPBDIV & 3) == 2) tmp >>= 1;
    tmp >>= 4;
    U1DLL = tmp & 0xFF;
    U1DLM = (tmp >> 8) & 0xFF;
    U1LCR = 0x03;                       /* DLAB = 0*/
}

void uart_putc(char c) {                /* Write character to Serial Port */
    while (!(U1LSR & 0x20));
    U1THR = c;
}

int uart_getc(void) {                   /* Read character from Serial Port */
    while (!(U1LSR & 0x01));
    return U1RBR;
}
