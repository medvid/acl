/* Parts taken from u-boot pl01x uart driver */

#include "pl011.h"
#include "serial.h"
#include "board.h"

void uart_init(void)
{
    uint temp;
    uint divider;
    uint remainder;
    uint fraction;

    /* Empty RX fifo if necessary */
    if (UART0_CR & UART_PL011_CR_UARTEN) {
        while (!(UART0_FR & UART_PL011_FR_RXFE))
            temp = UART0_DR;
    }

    /* Disable everythnig */
    UART0_CR = 0;

    /*
     * Set baud rate
     *
     * IBRD = UART_CLK / (16 * BAUD_RATE)
     * FBRD = RND((64 * MOD(UART_CLK,(16 * BAUD_RATE))) / (16 * BAUD_RATE))
     */
    temp = 16 * CONFIG_BAUDRATE;
    divider = CONFIG_PL011_CLOCK / temp;
    remainder = CONFIG_PL011_CLOCK % temp;
    temp = (8 * remainder) / CONFIG_BAUDRATE;
    fraction = (temp >> 1) + (temp & 1);

    UART0_IBRD = divider;
    UART0_FBRD = fraction;

    /* Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled */
    UART0_LCR_H = UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN;;

    /* Finally, enable the UART */
    UART0_CR = UART_PL011_CR_UARTEN | UART_PL011_CR_TXE | UART_PL011_CR_RXE |
               UART_PL011_CR_RTS;
}

void uart_putc(char c)
{
    /* Wait until there is space in the FIFO */
    while (UART0_FR & UART_PL011_FR_TXFF);

    /* Send the character */
    UART0_DR = c;
}

int uart_getc(void)
{
    uint data;

    /* Wait until there is data in the FIFO */
    while (UART0_FR & UART_PL011_FR_RXFE);

    data = UART0_DR;

    /* Check for an error flag */
    if (data & 0xFFFFFF00) {
        /* Clear the error */
        UART0_ECR = 0xFFFFFFFF;
        return -1;
    }

    return (int) data;
}
