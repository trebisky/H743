/* serial.c
 * Copied from /u1/Projects/STM32/F411/Archive/serial1/serial.c
 * 5-4-2025
 *
 * (c) Tom Trebisky  7-2-2017
 *
 * Serial (uart) driver for the F411
 * For the 411 this is section 19 of RM0383
 *
 * This began (2017) as a simple polled output driver for
 *  console messages on port 1
 * In 2020, I decided to extend it to listen to a GPS receiver
 *  on port 2.
 *
 * Notice that I number these 1,2,3.
 * However my "3" is what they call "6" in the manual.
 * 
 * On the F411, USART1 and USART6 are on the APB2 bus.
 * On the F411, USART2 is on the APB1 bus.
 *
 * On the F411, after reset, with no fiddling with RCC
 *  settings, both are running at 16 Mhz.
 *  Apparently on the F411 both APB1 and APB2
 *   always run at the same rate.
 */

// #include "f411.h"

typedef volatile unsigned int vu32;

/* This is the same register layout as the STM32F103 and F411
 * which is handy.
 */
struct uart {
        vu32 status;  /* 00 */
        vu32 data;    /* 04 */
        vu32 baud;    /* 08 */
        vu32 cr1;     /* 0c */
        vu32 cr2;     /* 10 */
        vu32 cr3;     /* 14 */
        vu32 gtp;     /* 18 - guard time and prescaler */
};

/* A first guess */
#define BUS_CLOCK	16000000

#define UART1   1
#define UART2   2
#define UART3   3

/* Here are the base addresses for the H743,
 *  which are the same as the F411
 */
#define UART1_BASE      (struct uart *) 0x40011000
#define UART2_BASE      (struct uart *) 0x40004400
#define UART3_BASE      (struct uart *) 0x40004800
#define UART4_BASE      (struct uart *) 0x40004C00
#define UART5_BASE      (struct uart *) 0x40005000
#define UART6_BASE      (struct uart *) 0x40011400

/* Bits in Cr1 */
#define	CR1_ENABLE	0x2000
#define	CR1_9BIT	0x1000
#define	CR1_WAKE	0x0800
#define	CR1_PARITY	0x0400
#define	CR1_ODD		0x0200
#define	CR1_PIE		0x0100
#define	CR1_TXEIE	0x0080
#define	CR1_TCIE	0x0040
#define	CR1_RXIE	0x0020
#define	CR1_IDLE_IE	0x0010
#define	CR1_TXE		0x0008
#define	CR1_RXE		0x0004
#define	CR1_RWU		0x0002
#define	CR1_BRK		0x0001

// I don't understand the 9 bit thing, but it is needed.
#define CR1_CONSOLE	0x340c

/* SAM_M8Q gps is 9600, no parity, one stop */
#define CR1_GPS		0x200c

/* bits in the status register */
#define	ST_PE		0x0001
#define	ST_FE		0x0002
#define	ST_NE		0x0004
#define	ST_OVER		0x0008
#define	ST_IDLE		0x0010
#define	ST_RXNE		0x0020		/* Receiver not empty */
#define	ST_TC		0x0040		/* Transmission complete */
#define	ST_TXE		0x0080		/* Transmitter empty */
#define	ST_BREAK	0x0100
#define	ST_CTS		0x0200

/* The baud rate.  This is subdivided from the bus clock.
 * It is as simple as dividing the bus clock by the baud
 * rate.  We could worry about it not dividing evenly, but
 * what can we do if it does not?
 */
static void
uart_init ( struct uart *up, int baud )
{
	/* 1 start bit, even parity */
	up->cr1 = CR1_CONSOLE;
	// up->cr1 = CR1_GPS;

	up->cr2 = 0;
	up->cr3 = 0;
	up->gtp = 0;

	up->baud = BUS_CLOCK / baud;
#ifdef notdef
	if ( up == UART2_BASE )
	    up->baud = get_pclk1() / baud;
	else
	    up->baud = get_pclk2() / baud;
#endif
}

void
serial_init ( void )
{
	gpio_uart_init ( UART3 );
	uart_init ( UART3_BASE, 115200 );
}

/* XXX locked onto UART3 */
void
console_putc ( int c )
{
	struct uart *up = UART3_BASE;

	if ( c == '\n' )
	    console_putc ( '\r' );

	while ( ! (up->status & ST_TXE) )
	    ;
	up->data = c;
}

/* Polled read (spins forever on console) */
int
console_getc ( void )
{
	struct uart *up = UART3_BASE;

	while ( ! (up->status & ST_RXNE) )
	    ;
	return up->data & 0x7f;
}


void
console_puts ( char *s )
{
	while ( *s )
	    console_putc ( *s++ );
}

/* Just for fun, recursive base 10 print
 */
void
printn ( int x )
{
	int d;

	if ( x == 0 )
	    return;
	if ( x < 0 ) {
	    console_putc ( '-' );
	    printn ( -x );
	    return;
	}
	d = x % 10;
	printn ( x / 10 );
	console_putc ( '0' + d );
}

void
show_n ( char *s, int val )
{
	console_puts ( s );
	console_putc ( ' ' );
	printn ( val );
	console_putc ( '\n' );
}

/* THE END */
