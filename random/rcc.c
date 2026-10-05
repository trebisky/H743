/* rcc.c
 *
 * (c) Tom Trebisky  10-1-2026 (H743)
 */

/* RCC (reset and clock control
 * 0x58024400 - 0x580247FF (RM page 138)
 *
 * We use RM0433 for the H743
 *   (use RM0399 for the H747)
 * See section 8 of the TRM for RCC details
 * PWR in section 6
 * Section 11 is gpio
 * page 129 (in section 2) has the
 *      memory map for the H743
 */

typedef volatile unsigned int vu32;
typedef unsigned int u32;

#define BIT(x)	(1<<(x))

#define RCC_BASE	(struct rcc *) 0x58024400
#define PWR_BASE	(struct power *) 0x58024800

// I am lazy and cheat for now, not using a struct for the RCC

/* The following take from my H747 project --
 * RCC registers are in 4 sections with banking and rules
 * about access from CPU1 or CPU2
 */
struct rcc {
	vu32	cr;			/* 00 */
	vu32	hsicfgr;	/* 04 */
	vu32	crrcr;		/* 08 */
	vu32	csicfgr;	/* 0c */
	vu32	cfgr;		/* 10 */
	u32	_pad0;
	vu32	d1cfgr;		/* 18 */
	vu32	d2cfgr;		/* 1c */
	vu32	d3cfgr;		/* 20 */
	u32	_pad1;
	vu32	pllckselr;	/* 28 */
	vu32	pllcfgr;	/* 2c */
	vu32	pll1divr;	/* 30 */
	vu32	pll1fracr;	/* 34 */
	vu32	pll2divr;	/* 38 */
	vu32	pll2fracr;	/* 3c */
	vu32	pll3divr;	/* 40 */
	vu32	pll3fracr;	/* 44 */
	u32	_pad2;
	vu32	d1ccipr;	/* 4c */
	vu32	d2ccip1r;	/* 50 */
	vu32	d2ccip2r;	/* 54 */
	vu32	d3ccipr;	/* 58 */
	u32	_pad3;
	vu32	cier;		/* 60 */
	vu32	cifr;		/* 64 */
	vu32	cicr;		/* 68 */
	u32	_pad4;
	vu32	bdcr;		/* 70 */
	vu32	csr;		/* 74 */
	u32	_pad5;
	vu32	ahb3rstr;	/* 7c */
	vu32	ahb1rstr;	/* 80 */
	vu32	ahb2rstr;	/* 84 */
	vu32	ahb4rstr;	/* 88 */
	vu32	apb3rstr;	/* 8c */
	vu32	apb1lrstr;	/* 90 */
	vu32	apb1hrstr;	/* 94 */
	vu32	apb2rstr;	/* 98 */
	vu32	apb4rstr;	/* 9c */
	vu32	gcr;		/* a0 */
	u32	_pad6;
	vu32	d3amr;		/* a8 */
	u32	_pad7[9];
	vu32	rsr;		/* d0 (aliases) */
	vu32	ahb3enr;	/* d4 (aliases) */
	vu32	ahb1enr;	/* d8 (aliases) */
	vu32	ahb2enr;	/* dc (aliases) */
	vu32	ahb4enr;	/* E0 (aliases) */
	vu32	apb3enr;	/* E4 (aliases) */
	vu32	apb1lenr;	/* E8 (aliases) */
	vu32	apb1henr;	/* Ec (aliases) */
	vu32	apb2enr;	/* F0 (aliases) */
	vu32	apb4enr;	/* F4 (aliases) */
	u32	_pad8;
	vu32	ahb3lpenr;	/* Fc (aliases) */
	vu32	ahb1lpenr;	/* 100 (aliases) */
	vu32	ahb2lpenr;	/* 104 (aliases) */
	vu32	ahb4lpenr;	/* 108 (aliases) */
	vu32	apb3lpenr;	/* 10c (aliases) */
	vu32	apb1llpenr;	/* 110 (aliases) */
	vu32	apb1hlpenr;	/* 114 (aliases) */
	vu32	apb2lpenr;	/* 118 (aliases) */
	vu32	apb4lpenr;	/* 11c (aliases) */
};

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
 * (the same for UART and the RNG)
 */

/* See section 8 of the TRM for RCC details */

#ifdef notdef
/* We are LAZY here and just pick out the
 * RCC registers we need for now.
 */
// #define RCC_BASE	(struct rcc *) 0x58024400

// RCC_AHB4ENR is at offset 0xE0
// I am cheating with the following

#define AHB1EN 0x580244d8
#define AHB2EN 0x580244dc
#define AHB4EN 0x580244e0
#define APB1EN 0x580244e8
#endif

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
	struct rcc *rp = RCC_BASE;
	vu32 *en;

	//en = (vu32 *) AHB4EN;
	//*en |= (GPIOA_ENABLE|GPIOB_ENABLE|GPIOC_ENABLE|GPIOD_ENABLE|GPIOE_ENABLE);
	rp->ahb4enr |= (GPIOA_ENABLE|GPIOB_ENABLE|GPIOC_ENABLE|GPIOD_ENABLE|GPIOE_ENABLE);

	// en = (vu32 *) APB1EN;
	// *en |= UART3_ENABLE;
	rp->apb1lenr |= UART3_ENABLE;

#define RND_RESET   BIT(6)
#define RND_ENA     BIT(6)
    // Get the true random number gadget going
    // rp->ahb2rstr |= RND_RESET;

	// en = (vu32 *) AHB2EN;
    // *en |= RND_ENA;
	rp->ahb2enr |= RND_ENA;

#ifdef notdef
	// TRM says these are all zero (not in reset)
	//  on power up.
	ahb4rst = 0;					/* 0x088 */
#endif
}

/* Our first guess was 96 Mhz, but
 *  stopwatch indicated more like 68 Mhz.
 * Searching says 64 Mhz "out of the box" on
 * all the H7 parts.  It is running on the internal
 * HSI RC oscillator, with all the PLL disabled/bypassed.
 */
#define CLOCK_64
// #define PCLK1           48000000
// #define PCLK2           96000000
#define CPU_HZ          64000000

int
get_cpu_hz ( void )
{
    return CPU_HZ;
}

/* THE END */
