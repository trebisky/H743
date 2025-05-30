/* blink.c
 * (c) Tom Trebisky  11-20-2020 (F411)
 * (c) Tom Trebisky  5-2-2025 (H743)
 */

/* int and long are both 4 bytes on 32 bit arm,
 * but are 4 and 8 on 64 bit, so beware.
 */
typedef volatile unsigned int vu32;
typedef unsigned int u32;

// #define STM32F1
// #define STM32F4
#define STM32F7

/* Changes to port this from F411 to H743
 *
 * LED on different pin.
 * all base addresses are different.
 * RCC is totally different.
 */

/* The F743 Nucleo board has 3 LED:
 * all are on GPIO B
 *  LD1 - Green - PB0 (PA5 via alternate jumper)
 *  LD2 - Orange (yellow) - PB1
 *  LD3 - Red - PB14
*/

#ifdef STM32F4
struct rcc {
	volatile unsigned int cr;	/* 0 - control reg */
	volatile unsigned int pll;	/* 4 - pll config */
	volatile unsigned int conf;	/* 8 - clock config */
	volatile unsigned int cir;	/* c - clock interrupt */
	volatile unsigned int ahb1_r;	/* 10 - AHB1 peripheral reset */
	volatile unsigned int ahb2_r;	/* 14 - AHB2 peripheral reset */
	int __pad1[2];
	volatile unsigned int apb1_r;	/* 20 - APB1 peripheral reset */
	volatile unsigned int apb2_r;	/* 24 - APB2 peripheral reset */
	int __pad2[2];
	volatile unsigned int ahb1_e;	/* 30 - AHB1 peripheral enable */
	volatile unsigned int ahb2_e;	/* 34 - AHB2 peripheral enable */
	int __pad3[2];
	volatile unsigned int apb1_e;	/* 40 - APB1 peripheral enable */
	volatile unsigned int apb2_e;	/* 44 - APB2 peripheral enable */
	int __pad4[2];
	volatile unsigned int ahb1_elp;	/* 50 - AHB1 peripheral enable in low power */
	volatile unsigned int ahb2_elp;	/* 54 - AHB2 peripheral enable in low power */
	int __pad5[2];
	volatile unsigned int apb1_elp;	/* 60 - APB1 peripheral enable in low power */
	volatile unsigned int apb2_elp;	/* 64 - APB2 peripheral enable in low power */
	int __pad6[2];
	volatile unsigned int bdcr;	/* 70 */
	volatile unsigned int csr;	/* 74 */
	int __pad7[2];
	volatile unsigned int sscgr;	/* 80 */
	volatile unsigned int plli2s;	/* 84 */
	int __pad8;
	volatile unsigned int dccr;	/* 8c */
};
#endif

#ifdef STM32F1
/* The reset and clock control module */
struct rcc {
	volatile unsigned long rc;	/* 0 - clock control */
	volatile unsigned long cfg;	/* 4 - clock config */
	volatile unsigned long cir;	/* 8 - clock interrupt */
	volatile unsigned long apb2;	/* c - peripheral reset */
	volatile unsigned long apb1;	/* 10 - peripheral reset */
	volatile unsigned long ape3;	/* 14 - peripheral enable */
	volatile unsigned long ape2;	/* 18 - peripheral enable */
	volatile unsigned long ape1;	/* 1c - peripheral enable */
	volatile unsigned long bdcr;	/* 20 - xx */
	volatile unsigned long csr;	/* 24 - xx */
};
#endif

#if defined(STM32F4) || defined(STM32F7)
#define GPIOA_ENABLE	0x01
#define GPIOB_ENABLE	0x02
#define GPIOC_ENABLE	0x04
#define GPIOD_ENABLE	0x08
#define GPIOE_ENABLE	0x10
#define GPIOH_ENABLE	0x80
#endif

#ifdef STM32F1
#define GPIOA_ENABLE	0x04
#define GPIOB_ENABLE	0x08
#define GPIOC_ENABLE	0x10
#endif

/* Identical for F4 and F7 */
#if defined(STM32F4) || defined(STM32F7)
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
#endif

#ifdef STM32F1
/* One of the 3 gpios */
struct gpio {
	vu32 cr[2];
	vu32 idr;
	vu32 odr;
	vu32 bsrr;
	vu32 brr;
	vu32 lock;
};
#endif

struct power {
	vu32 cr1;
	vu32 csr1;
	vu32 cr2;
	vu32 cr3;			/* bits for USB */
	vu32 cpucr;			/* 0x10 */
	u32	_pad1;
	vu32 d3cr;
	u32	_pad2;
	vu32 wkupcr;		/* 0x20 */
	vu32 wkupfr;
	vu32 wkupep;		/* 0x28 */
};

#ifdef STM32F7
#define RCC_BASE	(struct rcc *) 0x58024400
#define PWR_BASE	(struct power *) 0x58024800

#define GPIOA_BASE	(struct gpio *) 0x58020000
#define GPIOB_BASE	(struct gpio *) 0x58020400
#define GPIOC_BASE	(struct gpio *) 0x58020800
#endif

#ifdef STM32F4
#define RCC_BASE	(struct rcc *) 0x40023800

#define GPIOA_BASE	(struct gpio *) 0x40020000
#define GPIOB_BASE	(struct gpio *) 0x40020400
#define GPIOC_BASE	(struct gpio *) 0x40020800
#endif

#ifdef STM32F1
#define RCC_BASE	(struct rcc *) 0x40021000

#define GPIOA_BASE	(struct gpio *) 0x40010800
#define GPIOB_BASE	(struct gpio *) 0x40010C00
#define GPIOC_BASE	(struct gpio *) 0x40011000
#endif

#define MODE_OUT_2	0x02	/* Output, 2 Mhz */

#define CONF_GP_UD	0x0	/* Pull up/down */
#define CONF_GP_OD	0x4	/* Open drain */

/* On the STM32F103, this gives a blink rate of about 2.7 Hz */
/* i.e. the delay time is about 0.2 seconds (200 ms) */
#define FAST	200

#define FASTER	50
#define SLOWER	800

void
delay ( void )
{
	// volatile int count = 1000 * FAST;
	volatile unsigned int count = 1000 * SLOWER;

	while ( count-- )
	    ;
}

/* Only for H743 -- power control
 * Section 6 in the TRM
 */
void
power_setup ( void )
{
}

/* We have to enable the GPIO in the RCC registers
 * before we can use it
 * The chip powers up with all the gpio held in reset.
 */

/* See section 8 of the TRM for RCC details */
// Lazy
// #define RCC_BASE	(struct rcc *) 0x58024400
#define AHB4EN 0x580244e0

/* What the heck, enable A, B, and C for the H7 */
void
gpio_enable ( void )
{
	// struct rcc *rp = RCC_BASE;
	vu32 *en = (vu32 *) AHB4EN;

	// *en |= (GPIOA_ENABLE|GPIOA_ENABLE|GPIOA_ENABLE);
	*en |= (GPIOA_ENABLE|GPIOB_ENABLE|GPIOC_ENABLE);

#ifdef notdef
	// TRM says these are all zero (not in reset)
	//  on power up.
	ahb4rst = 0;					/* 0x088 */
#endif
}

#if defined(STM32F4) || defined(STM32F1)
void
gpio_enable ( void )
{
	struct rcc *rp = RCC_BASE;

#ifdef STM32F4
	rp->ahb1_e |= GPIOC_ENABLE;	/* enable the clock */
#else /* STM32F1 */
	rp->ape2 |= GPIOC_ENABLE;
#endif
}
#endif

#if defined(STM32F4) || defined(STM32F1)
/* F1 and F4 had the LED on C13 */
#define LED_GPIO_BASE	GPIOC_BASE
#define LED_GPIO_PIN	13
#endif

/* F743 Nucleo has an LED on B0 */
#ifdef STM32F7
#define LED_GPIO_BASE	GPIOB_BASE
#define LED_GPIO_PIN	0
#endif

struct gpio *gp;
u32 on_mask;
u32 off_mask;

void
led_init ( int bit )
{
	int conf;
	int shift;

	power_setup ();

	/* Turn on the GPIO in the RCC */
	gpio_enable ();

	gp = LED_GPIO_BASE;

#if defined(STM32F4) || defined(STM32F7)
	shift = bit * 2;
	gp->mode &= ~(3<<shift);
	gp->mode |= (1<<shift);
	gp->otype &= ~(1<<bit);
#else
// STM32F1
	shift = (bit - 8) * 4;
	conf = gp->cr[1] & ~(0xf<<shift);
	conf |= (MODE_OUT_2|CONF_GP_OD) << shift;
	gp->cr[1] = conf;
#endif

	on_mask = 1 << bit;
	off_mask = 1 << (bit+16);
}

void
led_on ( void )
{
	gp->bsrr = on_mask;
}

void
led_off ( void )
{
	gp->bsrr = off_mask;
}

void
startup ( void )
{
	led_init ( LED_GPIO_PIN );

	for ( ;; ) {
	    led_on ();
	    delay ();
	    led_off ();
	    delay ();
	}
}

/* THE END */
