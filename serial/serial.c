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

#define BIT(nr)		(1<<(nr))

/* This is the NEW register layout for the H743
 */
struct uart {
        vu32 cr1;  		/* 00 */
        vu32 cr2;    	/* 04 */
        vu32 cr3;    	/* 08 */
        vu32 baud;      /* 0c */
        vu32 gtp;       /* 10 */
        vu32 rto;       /* 14 */
        vu32 rqr;       /* 18 */
		vu32 isr;		/* 1c */
		vu32 icr;		/* 20 */
		vu32 rxdata;	/* 24 */
		vu32 txdata;	/* 28 */
		vu32 pre;		/* 2c */
};

#define CR1_UE		BIT(0)		/* enable UART */
#define CR1_TxE		BIT(3)		/* enable UART */
#define CR1_RxE		BIT(2)		/* enable UART */

#define PRE4		2		/* divide by 4 */

#define ISR_TXFNF	BIT(7)		/* Tx Fifo not full, OK to write */
#define ISR_RXFNE	BIT(5)		/* Rx Fifo not empty, data to read */

/* A wild first guess */
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

static void	uart_init ( struct uart *, int );

void
serial_init ( void )
{
	gpio_uart_init ( UART3 );
	uart_init ( UART3_BASE, 115200 );
}

static void
uart_init ( struct uart *up, int baud )
{
	/* 1 start bit, even parity */
	// XXX we never set any of this stuff.

	/* Start fresh and disable the uart
	 * This will set OVER8 = 0 (16x oversampling)
	 */
	up->cr1 = 0;

	up->cr2 = 0;
	up->cr3 = 0;
	up->gtp = 0;

	/* Divide the kerclk by 4 to get a 16Mhz clock
	 * So the kerclk must be 64 Mhz.
	 */
	up->pre = PRE4;

	/* This runs at 460800 with the prescaler out of the game */
	up->baud = BUS_CLOCK / baud;

	up->cr1 |= (CR1_TxE | CR1_RxE);

	/* Enable the uart */
	up->cr1 |= CR1_UE;
}

/* XXX locked onto UART3 */
void
console_putc ( int c )
{
	struct uart *up = UART3_BASE;

	if ( c == '\n' )
	    console_putc ( '\r' );

	while ( ! ( up->isr & ISR_TXFNF ) )
		;
	up->txdata = c;
}

void
console_puts ( char *s )
{
	while ( *s )
	    console_putc ( *s++ );
}

int
console_getc ( void )
{
	struct uart *up = UART3_BASE;

	while ( ! (up->isr & ISR_RXFNE) )
	    ;
	return up->rxdata & 0x7f;
}

/* Return 1 if something is waiting to be read */
int
console_check ( void )
{
	struct uart *up = UART3_BASE;

	if ( up->isr & ISR_RXFNE)
		return 1;
	return 0;
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
