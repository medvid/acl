#ifndef PL011_H
#define PL011_H

/* PrimeCell UART (PL011) */

#define UART_BASE_ADDR(ch)      (0x101F1000+0x1000*(ch))
#define UART0_BASE_ADDR         UART_BASE_ADDR(0)

#define UART0_DR                VPint(UART0_BASE_ADDR+0x00)
#define UART0_ECR               VPint(UART0_BASE_ADDR+0x04)
#define UART0_FR                VPint(UART0_BASE_ADDR+0x18)
#define UART0_IBRD              VPint(UART0_BASE_ADDR+0x24)
#define UART0_FBRD              VPint(UART0_BASE_ADDR+0x28)
#define UART0_LCR_H             VPint(UART0_BASE_ADDR+0x2C)
#define UART0_CR                VPint(UART0_BASE_ADDR+0x30)

#define UART_PL011_RSR_OE       0x08
#define UART_PL011_RSR_BE       0x04
#define UART_PL011_RSR_PE       0x02
#define UART_PL011_RSR_FE       0x01

#define UART_PL011_FR_TXFE      0x80
#define UART_PL011_FR_RXFF      0x40
#define UART_PL011_FR_TXFF      0x20
#define UART_PL011_FR_RXFE      0x10
#define UART_PL011_FR_BUSY      0x08
#define UART_PL011_FR_TMSK      (UART_PL011_FR_TXFF + UART_PL011_FR_BUSY)

#define UART_PL011_LCRH_SPS     (1 << 7)
#define UART_PL011_LCRH_WLEN_8  (3 << 5)
#define UART_PL011_LCRH_WLEN_7  (2 << 5)
#define UART_PL011_LCRH_WLEN_6  (1 << 5)
#define UART_PL011_LCRH_WLEN_5  (0 << 5)
#define UART_PL011_LCRH_FEN     (1 << 4)
#define UART_PL011_LCRH_STP2    (1 << 3)
#define UART_PL011_LCRH_EPS     (1 << 2)
#define UART_PL011_LCRH_PEN     (1 << 1)
#define UART_PL011_LCRH_BRK     (1 << 0)

#define UART_PL011_CR_CTSEN     (1 << 15)
#define UART_PL011_CR_RTSEN     (1 << 14)
#define UART_PL011_CR_OUT2      (1 << 13)
#define UART_PL011_CR_OUT1      (1 << 12)
#define UART_PL011_CR_RTS       (1 << 11)
#define UART_PL011_CR_DTR       (1 << 10)
#define UART_PL011_CR_RXE       (1 << 9)
#define UART_PL011_CR_TXE       (1 << 8)
#define UART_PL011_CR_LPE       (1 << 7)
#define UART_PL011_CR_IIRLP     (1 << 2)
#define UART_PL011_CR_SIREN     (1 << 1)
#define UART_PL011_CR_UARTEN    (1 << 0)

#define UART_PL011_CR_CTSEN     (1 << 15)
#define UART_PL011_CR_RTSEN     (1 << 14)
#define UART_PL011_CR_OUT2      (1 << 13)
#define UART_PL011_CR_OUT1      (1 << 12)
#define UART_PL011_CR_RTS       (1 << 11)
#define UART_PL011_CR_DTR       (1 << 10)
#define UART_PL011_CR_RXE       (1 << 9)
#define UART_PL011_CR_TXE       (1 << 8)
#define UART_PL011_CR_LPE       (1 << 7)
#define UART_PL011_CR_IIRLP     (1 << 2)
#define UART_PL011_CR_SIREN     (1 << 1)
#define UART_PL011_CR_UARTEN    (1 << 0)

#endif
