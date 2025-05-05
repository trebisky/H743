/* gpio.c
 *
 * Copied from /u1/Projects/STM32/F411/Archive/serial1/gpio.c
 *  5-4-2025
 *
 * (c) Tom Trebisky  9-24-2016
 * (c) Tom Trebisky  11-20-2020
 *
 * Basic GPIO driver for the F411 and H743
 *
 * Also includes LED routines
 */

// #include "f411.h"
#include "h743.h"

typedef volatile unsigned int vu32;

#ifdef WANT_LED
/* Where is the LED ?? */
#define LED_PIN		13	/* PC13 */
#define LED_GPIO	GPIOC	/* PC13 */
#endif

/* Here is the F411 gpio structure.
 *  very different from the F103.
 */
struct gpio {
	vu32 mode;		/* 0x00 */
	vu32 otype;		/* 0x04 */
	vu32 ospeed;	/* 0x08 */
	vu32 pupd;		/* 0x0c */
	vu32 idata;		/* 0x10 */
	vu32 odata;		/* 0x14 */
	vu32 bsrr;		/* 0x18 */
	vu32 lock;		/* 0x1c */
	vu32 afl;		/* 0x20 */
	vu32 afh;		/* 0x24 */
};

#ifdef CHIP_F411
#define GPIOA_BASE	(struct gpio *) 0x40020000
#define GPIOB_BASE	(struct gpio *) 0x40020400
#define GPIOC_BASE	(struct gpio *) 0x40020800
#endif

#define GPIOA_BASE	(struct gpio *) 0x58020000
#define GPIOB_BASE	(struct gpio *) 0x58020400
#define GPIOC_BASE	(struct gpio *) 0x58020800
#define GPIOD_BASE	(struct gpio *) 0x58020C00

static struct gpio *gpio_bases[] = {
    GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE
};

/* Change alternate function setting for a pin
 * These are 4 bit fields. All initially 0.
 */
void
gpio_af ( int gpio, int pin, int val )
{
	struct gpio *gp;
	int shift;

	gp = gpio_bases[gpio];

	if ( pin < 8 ) {
	    shift = pin * 4;
	    gp->afl &= ~(0xf<<shift);
	    gp->afl |= val<<shift;
	} else {
	    shift = (pin-8) * 4;
	    gp->afh &= ~(0xf<<shift);
	    gp->afh |= val<<shift;
	}
}

/* This is a 2 bit field */
void
gpio_mode ( int gpio, int pin, int val )
{
	struct gpio *gp;
	int shift;

	gp = gpio_bases[gpio];
	shift = pin * 2;
	gp->mode &= ~(0x3<<shift);
	gp->mode |= val<<shift;
}

/* kludge for now */
void
gpio_uart ( int gpio, int pin, int val )
{
	struct gpio *gp;
	int shift;

	/* XXX ignores val */
	gp = gpio_bases[gpio];

	gp->otype &= ~(1<<pin);

	shift = pin * 2;
	gp->ospeed |= (3<<shift);

	gp->pupd &= ~(3<<shift);
}

/* For the H743, we take short cuts for now */
void
gpio_uart_init ( int uart )
{
	    gpio_af ( GPIOD, 8, 7 );	/* Tx */
	    gpio_mode ( GPIOD, 8, 2 );
	    gpio_uart ( GPIOD, 8, 99 );

	    gpio_af ( GPIOD, 9, 7 );	/* Rx */
	    gpio_mode ( GPIOD, 9, 2 );
	    gpio_uart ( GPIOD, 9, 99 );
}

#ifdef notdef
/* Note that UART1 can be moved around a lot.
 * I make a choice here.
 * I suppose a general interface would allow this to
 *  be selected via a call argument.
 */
void
gpio_uart_init ( int uart )
{
	if ( uart == UART1 ) {
	    gpio_af ( GPIOA, 9, 7 );	/* Tx */
	    gpio_mode ( GPIOA, 9, 2 );	/* Tx */
	    gpio_uart ( GPIOA, 9, 99 );	/* Tx */

	    gpio_af ( GPIOA, 10, 7 );	/* Rx */
	    gpio_mode ( GPIOA, 10, 2 );
	    gpio_uart ( GPIOA, 10, 99 );
	    // gpio_af ( GPIOA, 15, 7 ); /* Tx */
	    // gpio_af ( GPIOB, 3, 7 );	/* Rx */
	    // gpio_af ( GPIOB, 6, 7 )	/* Tx */
	    // gpio_af ( GPIOB, 7, 7 );	/* Rx */
	} else if ( uart == UART2 ) {
	    gpio_af ( GPIOA, 2, 7 );	/* Tx */
	    gpio_af ( GPIOA, 3, 7 );	/* Rx */
	} else { /* UART3 */
	    gpio_af ( GPIOC, 6, 7 );	/* Tx */
	    gpio_af ( GPIOC, 7, 7 );	/* Rx */
	}
}
#endif

/* ========================================================== */

#ifdef WANT_LED
static struct gpio *led_gp;
static unsigned long on_mask;
static unsigned long off_mask;

void
led_init ( void )
{
	int conf;
	int shift;
	int pin = LED_PIN;

	// led_gp = GPIOC_BASE;
	led_gp = gpio_bases[LED_GPIO];

	shift = pin * 2;
	led_gp->mode &= ~(3<<shift);
	led_gp->mode |= (1<<shift);
	led_gp->otype &= ~(1<<pin);

	off_mask = 1 << pin;
	on_mask = 1 << (pin+16);
}

void
led_on ( void )
{
	led_gp->bsrr = on_mask;
}

void
led_off ( void )
{
	led_gp->bsrr = off_mask;
}
#endif

/* THE END */
