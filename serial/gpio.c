/* gpio.c
 *
 * Copied from /u1/Projects/STM32/F411/Archive/serial1/gpio.c
 *  5-4-2025
 *
 * (c) Tom Trebisky  9-24-2016
 * (c) Tom Trebisky  11-20-2020
 * (c) Tom Trebisky  9-30-2026
 *
 * Basic GPIO driver for the F411 and H743
 */

#include "h743.h"

typedef volatile unsigned int vu32;

/* Here is the F411 gpio structure.
 *  very different from the F103.
 * Seems to work for the H743
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

/* THE END */
