/* rcc.c
 *
 * (c) Tom Trebisky  10-1-2026 (H743)
 */

typedef volatile unsigned int vu32;
typedef unsigned int u32;

/* Section 11 of the TRM is gpio
 * page 129 has the memory map for the H743
 */
#define RCC_BASE	(struct rcc *) 0x58024400
#define PWR_BASE	(struct power *) 0x58024800

// I am lazy and chat for now, no using a struct for the RCC

#ifdef notdef
/* This is the RCC for the F411, who knows for the H743 ...
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

#ifdef notdef
/* For the H7 -- never used */
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

/* We have to enable the GPIO in the RCC registers
 * before we can use it
 * The chip powers up with all the gpio held in reset.
 */

/* See section 8 of the TRM for RCC details */
/* We are LAZY here and just pick out the two
 * RCC registers we need for now.
 */
// #define RCC_BASE	(struct rcc *) 0x58024400

#define AHB4EN 0x580244e0
#define APB1EN 0x580244e8

#define UART3_ENABLE	0x40000

#define GPIOA_ENABLE	0x01
#define GPIOB_ENABLE	0x02
#define GPIOC_ENABLE	0x04
#define GPIOD_ENABLE	0x08
#define GPIOE_ENABLE	0x10
#define GPIOH_ENABLE	0x80


/* Enable gpio and uart for the H7 */
void
rcc_init ( void )
{
	// struct rcc *rp = RCC_BASE;
	vu32 *en;

	en = (vu32 *) AHB4EN;
	*en |= (GPIOA_ENABLE|GPIOB_ENABLE|GPIOC_ENABLE|GPIOD_ENABLE|GPIOE_ENABLE);

	en = (vu32 *) APB1EN;
	*en |= UART3_ENABLE;

#ifdef notdef
	// TRM says these are all zero (not in reset)
	//  on power up.
	ahb4rst = 0;					/* 0x088 */
#endif
}

/* THE END */
