/* main.c
 * (c) Tom Trebisky  11-20-2020 (F411)
 * (c) Tom Trebisky  5-2-2025 (H743)
 *
 * This was blink.c from the blink1 demo.
 * Here I have removed all the F103 and F411 code
 * and renamed it main.c
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

#if defined(STM32F4) || defined(STM32F7)
#define GPIOA_ENABLE	0x01
#define GPIOB_ENABLE	0x02
#define GPIOC_ENABLE	0x04
#define GPIOD_ENABLE	0x08
#define GPIOE_ENABLE	0x10
#define GPIOH_ENABLE	0x80
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

#ifdef notdef
/* For the F7 -- never used */
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
#endif

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

#define MODE_OUT_2	0x02	/* Output, 2 Mhz */

#define CONF_GP_UD	0x0	/* Pull up/down */
#define CONF_GP_OD	0x4	/* Open drain */

/* On the STM32F103, this gives a blink rate of about 2.7 Hz */
/* i.e. the delay time is about 0.2 seconds (200 ms) */
#define FAST	200

#define FASTER	50
#define SLOWER	800
#define SLOWER2	2000

#ifdef notdef
/* Thingsrun slow when this is up here,
 * but moved down below, we run normally (fast).
 * Mighty wierd.
 */
static void
delay ( void )
{
	// volatile int count = 1000 * FAST;
	// volatile unsigned int count = 1000 * SLOWER;
	volatile unsigned int count = 1000 * SLOWER2;

	while ( count-- )
	    ;
}
#endif


/* We have to enable the GPIO in the RCC registers
 * before we can use it
 * The chip powers up with all the gpio held in reset.
 */

/* See section 8 of the TRM for RCC details */
// Lazy
// #define RCC_BASE	(struct rcc *) 0x58024400

#define AHB4EN 0x580244e0
#define APB1EN 0x580244e8

#define UART3_ENABLE	0x40000

/* Enable A, B, C and D for the H7 */
void
gpio_enable ( void )
{
	// struct rcc *rp = RCC_BASE;
	vu32 *en;

	en = (vu32 *) AHB4EN;
	*en |= (GPIOA_ENABLE|GPIOB_ENABLE|GPIOC_ENABLE|GPIOD_ENABLE);

	en = (vu32 *) APB1EN;
	*en |= UART3_ENABLE;

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

char alpha[60];
char num[20];

static void
string_init ( void )
{
	int i;
	char *p;

	for ( i=0; i<10; i++ )
		num[i] = '0' + i;
	num[10] = '\n';
	num[11] = '\0';

	p = alpha;
	for ( i=0; i<26; i++ )
		*p++ = 'A' + i;
	for ( i=0; i<26; i++ )
		*p++ = 'a' + i;
	*p++ = '\n';
	*p = '\0';
}

char *msg1 = "Got";

static void
check ( void )
{
	int x;

	if ( ! console_check() )
		return;

	x = console_getc ();
	// OK
	// show_n ( msg1, x );
	// poison
	// show_n ( "Got:", x );
	console_puts ( "Stop\n" );
	show_n ( "Got:", x );

	while ( ! console_check() )
		;
	x = console_getc ();
	show_n ( "Got:", x );
	// show_n ( msg1, x );
	console_puts ( "Go\n" );
}

static void
delay ( void )
{
	// volatile int count = 1000 * FAST;
	// volatile unsigned int count = 1000 * SLOWER;
	volatile unsigned int count = 1000 * SLOWER2;

	while ( count-- )
	    ;
}

void
startup ( void )
{
	int n;

	led_init ( LED_GPIO_PIN );
	serial_init ();
	string_init ();

	n = 0;
	for ( ;; ) {
		show_n ( "Tick:", ++n );
	    led_on ();
		// console_putc ( 'X' );
		console_puts ( alpha );
		check ();
	    delay ();
		show_n ( "Tick:", ++n );
	    led_off ();
		// console_putc ( 'Y' );
		console_puts ( num );
		check ();
	    delay ();
	}
}

/* THE END */
