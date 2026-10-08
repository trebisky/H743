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
typedef unsigned int u32;

/* Here is the F411 gpio structure.
 *  very different from the F103.
 * Same for the H743
 *
 * Section 11 of the TRM is gpio
 * page 129 has the memory map for the H743
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
#define GPIOD_BASE	(struct gpio *) 0x58020c00
#define GPIOE_BASE	(struct gpio *) 0x58021000
#define GPIOF_BASE	(struct gpio *) 0x58021400
#define GPIOG_BASE	(struct gpio *) 0x58021800
#define GPIOH_BASE	(struct gpio *) 0x58021c00
#define GPIOI_BASE	(struct gpio *) 0x58022000
#define GPIOJ_BASE	(struct gpio *) 0x58022400
#define GPIOK_BASE	(struct gpio *) 0x58022800

static struct gpio *gpio_bases[] = {
    GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE,
    GPIOE_BASE, GPIOF_BASE, GPIOG_BASE, GPIOH_BASE,
    GPIOI_BASE, GPIOJ_BASE, GPIOK_BASE
};

#ifdef notdef
#define MODE_OUT_2	0x02	/* Output, 2 Mhz */

#define CONF_GP_UD	0x0	/* Pull up/down */
#define CONF_GP_OD	0x4	/* Open drain */
#endif

/* Change alternate function setting for a pin
 * These are 4 bit fields. All initially 0.
 */
static void
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
static void
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
static void
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

/* For the H743, we take short cuts for now.
 * -- called from serial.c
 */
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

/* ========================================================== */

/* The H743 Nucleo board has 3 LED:
 *  LD1 - Green - PB0 (PA5 via alternate jumper)
 *  LD2 - Orange (yellow) - PE1
 *  LD3 - Red - PB14
*/

#define LED1_GPIO_PIN	0	/* in GPIOB */
#define LED2_GPIO_PIN	1	/* in GPIOE */
#define LED3_GPIO_PIN	14	/* in GPIOB */

#ifdef notdef
struct gpio *gp;
u32 on_mask;
u32 off_mask;

struct gpio *gpx;
u32 xon_mask;
u32 xoff_mask;
#endif

struct gpio *gp1;
struct gpio *gp2;
struct gpio *gp3;

u32 on1_mask;
u32 on2_mask;
u32 on3_mask;

u32 off1_mask;
u32 off2_mask;
u32 off3_mask;

void
led_init ( void )
{
	int conf;
	int shift;
	int mask;
	int bit;

	gp1 = GPIOB_BASE;
	gp2 = GPIOE_BASE;
	gp3 = GPIOB_BASE;

	bit = LED1_GPIO_PIN;
	shift = bit * 2;
	gp1->mode &= ~(3<<shift);
	gp1->mode |= (1<<shift);
	gp1->otype &= ~(1<<bit);

	bit = LED3_GPIO_PIN;
	shift = bit * 2;
	gp3->mode &= ~(3<<shift);
	gp3->mode |= (1<<shift);
	gp3->otype &= ~(1<<bit);

	bit = LED2_GPIO_PIN;
	shift = bit * 2;
	gp2->mode &= ~(3<<shift);
	gp2->mode |= (1<<shift);
	gp2->otype &= ~(1<<bit);

//	on_mask = 1 << bit;
//	off_mask = 1 << (bit+16);

	mask = 1 << LED1_GPIO_PIN;
	on1_mask = mask;
	off1_mask = mask << 16;

	mask = 1 << LED3_GPIO_PIN;
	on3_mask = mask;
	off3_mask = mask << 16;

	mask = 1 << LED2_GPIO_PIN;
	on2_mask = mask;
	off2_mask = mask << 16;

	/* Turn all 3 off */
	gp1->bsrr = off1_mask;
	gp2->bsrr = off2_mask;
	gp3->bsrr = off3_mask;
}

void
led_on ( void )
{
	gp1->bsrr = on1_mask;
}

void
led_off ( void )
{
	gp1->bsrr = off1_mask;
}

static int state = 0;

/* Typically called on each systick
 */
void
led_next ( void )
{
#ifdef TRIPLE
	if ( state == 0 ) {
		state = 1;
		gp1->bsrr = on1_mask;
		gp2->bsrr = off2_mask;
		gp3->bsrr = off3_mask;
	}
	else if ( state == 1 ) {
		state = 2;
		gp1->bsrr = off1_mask;
		gp2->bsrr = on2_mask;
		gp3->bsrr = off3_mask;
	}
	else {
		state = 0;
		gp1->bsrr = off1_mask;
		gp2->bsrr = off2_mask;
		gp3->bsrr = on3_mask;
	}
#else
	if ( state == 0 ) {
		state = 1;
		gp1->bsrr = on1_mask;
	} else {
		state = 0;
		gp1->bsrr = off1_mask;
	}
#endif
}


/* THE END */
