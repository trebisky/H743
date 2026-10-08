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

// Without this, we build for the H747 Discovery
#define NUCLEO

#define RCC_BASE	(struct rcc *) 0x58024400

static void pwr_setup ( void );

/* The following taken from my H747 project --
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

/* These will be in BSS area.
 * For debugging
 */
volatile u32 gdb0;
volatile u32 gdb1;
volatile u32 gdb2;
volatile u32 gdb3;

#define modreg(reg, clr, set ) ( reg = ((reg & (~clr)) | set) )
// #define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

static void
turn_on ( u32 on, u32 rdy )
{
    struct rcc *rp = (struct rcc *) RCC_BASE;
    int tmo = 40000;

    rp->cr |= on;
    while ( tmo-- ) {
        if ( rp->cr & rdy )
        break;
    }
}

static void
turn_off ( u32 on, u32 rdy )
{
    struct rcc *rp = (struct rcc *) RCC_BASE;
    int tmo = 40000;

    rp->cr &= ~on;
    while ( tmo-- ) {
        if ( ! (rp->cr & rdy) )
        break;
    }
}

static void
show_reg ( char *msg, vu32 *addr )
{
    printf ( "%s %X %X\n", msg, (int) addr, *addr );
}

/* show (print) various interesting RCC settings
 * cannot call from rcc_init()
 *  (serial port not yet set up)
 */
void
rcc_show ( void )
{
	struct rcc *rp = RCC_BASE;

	/* In this register are the HSIDIV and HSION bits
	 */
	show_reg ( "RCC cr", &rp->cr );

	/* This register has the 4-way selection of the clock
	 * that feeds "sys_ck"
	 * It is 0 on reset, which selects HSI
	 */
	show_reg ( "RCC cfgr", &rp->cfgr );

	/* This has the usart1 selector */
	show_reg ( "RCC d2ccip2r", &rp->d2ccip2r );

	/* This has xx */
	show_reg ( "RCC d1cfgr", &rp->d1cfgr );
	show_reg ( "RCC d2cfgr", &rp->d2cfgr );
	show_reg ( "RCC d3cfgr", &rp->d3cfgr );
}

/* We have to enable the GPIO in the RCC registers
 * before we can use it
 * The chip powers up with all the gpio held in reset.
 * (the same for UART and the RNG)
 */

/* See section 8 of the TRM for RCC details */

#define UART3_ENABLE	0x40000

#define GPIOA_ENABLE	0x01
#define GPIOB_ENABLE	0x02
#define GPIOC_ENABLE	0x04
#define GPIOD_ENABLE	0x08
#define GPIOE_ENABLE	0x10
#define GPIOH_ENABLE	0x80

#define GPIO_ALL_ENA	0x7ff

/* Enable gpio and uart for the H7 */
static void
rcc_enables ( void )
{
	struct rcc *rp = RCC_BASE;

	rp->ahb4enr |= (GPIOA_ENABLE|GPIOB_ENABLE|GPIOC_ENABLE|GPIOD_ENABLE|GPIOE_ENABLE);

	rp->apb1lenr |= UART3_ENABLE;

#define RND_RESET   BIT(6)
#define RND_ENA     BIT(6)
    // Get the true random number gadget going
    // rp->ahb2rstr |= RND_RESET;
	rp->ahb2enr |= RND_ENA;

#ifdef notdef
	// TRM says these are all zero (not in reset)
	//  on power up.
	rp->ahb4rst = 0;
#endif

	/* Enable the SYSCFG clock.
	 * This is needed to allow us to fiddle with
	 * overdrive/voltage-scaling modes (VOS0)
	 * that will allow us to run at 480 Mhz
	 */
#define SYSCFG_ENA	BIT(1)
	rp->apb4enr |= SYSCFG_ENA;;
}

/*
 * On the H747 Discovery
 * HSI runs at 64 Mhz (can be divided)
 * HSE runs at 25 Mhz (external crystal)
 *
 * On the H743 Nucleo
 * HSI runs at 64 Mhz (can be divided)
 * HSE runs at 8 Mhz (from St-Link MCO)
 */

static void
rcc_pll_480 ( void )
{
	struct rcc *rp = (struct rcc *) RCC_BASE;
	int tmo;
	u32 val;

#define CR_HSI48_ON		BIT(12)
#define CR_HSI48_RDY	BIT(13)

#define CR_HSE_ON	BIT(16)
#define CR_HSE_RDY	BIT(17)

#define CR_PLL1_ON	BIT(24)
#define CR_PLL1_RDY	BIT(25)
#define CR_PLL2_ON	BIT(26)
#define CR_PLL2_RDY	BIT(27)
#define CR_PLL3_ON	BIT(28)
#define CR_PLL3_RDY	BIT(29)


#define	CR_HSIDIV_MASK	(0x3<<3)
#define	CR_HSIDIV1	(0x0<<3)
#define	CR_HSIDIV2	(0x1<<3)
#define	CR_HSIDIV4	(0x2<<3)
#define	CR_HSIDIV8	(0x3<<3)

	/* No MCO, make HSI the system clock.
	 * Do this before killing PLL
	 */
	rp->cfgr = 0;

	turn_off ( CR_PLL1_ON, CR_PLL1_RDY );
	turn_off ( CR_PLL2_ON, CR_PLL2_RDY );
	turn_off ( CR_PLL3_ON, CR_PLL3_RDY );

	/* The base HSI clock is 64 Mhz.
	 * divide by 4 to get 16 Mhz
	 * Must turn off PLL before doing this.
	 */
	// modreg ( rp->cr, CR_HSIDIV_MASK, CR_HSIDIV4 );
	modreg ( rp->cr, CR_HSIDIV_MASK, CR_HSIDIV8 );
	// modreg ( rp->cr, CR_HSIDIV_MASK, CR_HSIDIV1 );

	gdb0 = 0xdeadbeef;
	gdb1 = rp->cr;

	/* Turn on HSE */
	/* We see this count down from 40000 to 38834
	 */
	turn_on ( CR_HSE_ON, CR_HSE_RDY );

	/* Why not?  Turn on HSI48 also */
	/* We see this count down from 40000 to 39981
	 */
	turn_on ( CR_HSI48_ON, CR_HSI48_RDY );

	gdb2 = rp->cr;

	/* All PLL must be off to be able to mess with
	 * the pllckselr.
	 */

	/* All 3 PLL get the same clock,
	 * we select HSE
	 */
#define PLLCK_SEL_MASK	0x3
#define PLLCK_SEL_HSE	0x2
	modreg ( rp->pllckselr, PLLCK_SEL_MASK, PLLCK_SEL_HSE );

#define PLLCK_DIV1_MASK	(0x3f << 4)
#define PLLCK_DIV2_MASK	(0x3f << 12)
#define PLLCK_DIV3_MASK	(0x3f << 20)

#ifdef NUCLEO
#define PLLCK_DIV1_1	(1 << 4)
#define PLLCK_DIV2_1	(1 << 12)
#define PLLCK_DIV3_1	(1 << 20)

	/* All 3 PLL get HSE/1 (i.e. 8 Mhz)  */
	modreg ( rp->pllckselr, PLLCK_DIV1_MASK, PLLCK_DIV1_1 );
	modreg ( rp->pllckselr, PLLCK_DIV2_MASK, PLLCK_DIV2_1 );
	modreg ( rp->pllckselr, PLLCK_DIV3_MASK, PLLCK_DIV3_1 );
#else
#define PLLCK_DIV1_5	(5 << 4)
#define PLLCK_DIV2_5	(5 << 12)
#define PLLCK_DIV3_5	(5 << 20)

	/* All 3 PLL get HSE/5 (i.e. 5 Mhz)  */
	modreg ( rp->pllckselr, PLLCK_DIV1_MASK, PLLCK_DIV1_5 );
	modreg ( rp->pllckselr, PLLCK_DIV2_MASK, PLLCK_DIV2_5 );
	modreg ( rp->pllckselr, PLLCK_DIV3_MASK, PLLCK_DIV3_5 );
#endif

	gdb3 = rp->pllckselr;

#define PLLDIV_R_SHIFT	24
#define PLLDIV_Q_SHIFT	16
#define PLLDIV_P_SHIFT	9

/* The following is a final divide by 1 on the output */

#ifdef NUCLEO
#define PLLDIV0		0	// divide by 1
#define PLLMUL		59	// 8 * 60 = 480
#else
#define PLLDIV0		0	// divide by 1
#define PLLMUL		95	// 5 * 96 = 480
#endif

	/* Configure those PLL
	 * 5 Mhz * 96 - 480 Mhz
	 * 8 Mhz * 60 - 480 Mhz
	 */
	rp->pll1divr =
	    (PLLDIV0 << PLLDIV_R_SHIFT) |
	    (PLLDIV0 << PLLDIV_Q_SHIFT) |
	    (PLLDIV0 << PLLDIV_P_SHIFT) | PLLMUL;
	rp->pll1fracr = 0;

	rp->pll2divr =
	    (PLLDIV0 << PLLDIV_R_SHIFT) |
	    (PLLDIV0 << PLLDIV_Q_SHIFT) |
	    (PLLDIV0 << PLLDIV_P_SHIFT) | PLLMUL;
	rp->pll2fracr = 0;

	rp->pll3divr =
	    (PLLDIV0 << PLLDIV_R_SHIFT) |
	    (PLLDIV0 << PLLDIV_Q_SHIFT) |
	    (PLLDIV0 << PLLDIV_P_SHIFT) | PLLMUL;
	rp->pll3fracr = 0;

	/* the pllcfgr has VCO control fields along with
	 * enables for the various divider outputs
	 */
#define PLL1_FRACEN	BIT(0)
#define PLL2_FRACEN	BIT(4)
#define PLL3_FRACEN	BIT(8)

/* 0 is wide (192-836), 1 is medium (150-420)
 */
#define PLL1_WIDE	BIT(1)
#define PLL2_WIDE	BIT(5)
#define PLL3_WIDE	BIT(9)

#define PLL1_R_12	0
#define PLL1_R_24	(1<<2)
#define PLL1_R_48	(2<<2)
#define PLL1_R_816	(3<<2)

#define PLL2_R_12	0
#define PLL2_R_24	(1<<6)
#define PLL2_R_48	(2<<6)
#define PLL2_R_816	(3<<6)

#define PLL3_R_12	0
#define PLL3_R_24	(1<<10)
#define PLL3_R_48	(2<<10)
#define PLL3_R_816	(3<<10)

#define PLL1_PEN	BIT(16)
#define PLL1_QEN	BIT(17)
#define PLL1_REN	BIT(18)
#define PLL2_PEN	BIT(19)
#define PLL2_QEN	BIT(20)
#define PLL2_REN	BIT(21)
#define PLL3_PEN	BIT(22)
#define PLL3_QEN	BIT(23)
#define PLL3_REN	BIT(24)
#define PLL_PQR_ALL	(0x1f<<16)

/* We feed them all 5 or 8 Mhz and ask for 480 out */
#define PLL_VCO		PLL1_R_48 | PLL2_R_48 | PLL3_R_48
#define PLL_VCO_MASK	0x0fff

	modreg ( rp->pllcfgr, PLL_VCO_MASK, PLL_VCO );
	rp->pllcfgr |= PLL_PQR_ALL;

	/* Turn them on
	 */
	turn_on ( CR_PLL1_ON, CR_PLL1_RDY );
	turn_on ( CR_PLL2_ON, CR_PLL2_RDY );
	turn_on ( CR_PLL3_ON, CR_PLL3_RDY );

/* Now the sys_clk is running at 480 and we want
 * to set up the various bus clocks.
 */

	/* D1CPRE is the first thing that divides the sys clock.
	 * we leave it at 0 so we keep the full 480 for CPU1.
	 * HPRE will then divide by 2 so we get 240 for CPU2.
	 * We set D1PRE to 4 to send 60 Mhz to APB3/AHB3.
	 */
#define D1_CPRE_1	0
#define D1_HPRE_2	8
#define D1_PRE_4	(5<<4)
	/* Set HPRE to 2 so we get a 240 Mhz clock for CPU2
	 * as well as the various unscaled peripheral clocks
	 * This is APB3
	 */
	rp->d1cfgr = D1_HPRE_2 | D1_PRE_4;

	/* For the D2 domain we also divide the peripheral clocks
	 * down by 4 to get 60 Mhz.
	 * This is APB2 and APB1
	 */
#define D2_PRE1_4	(5<<4)
#define D2_PRE2_4	(5<<8)
	rp->d2cfgr = D2_PRE1_4 | D2_PRE2_4;

	/* For the D3 domain we also divide the peripheral clock
	 * down by 4 to get 60 Mhz.
	 * this is APB4
	 */
#define D3_PRE_4	(5<<4)
	rp->d3cfgr = D3_PRE_4;

	/* Switch the system clock from HSI to PLL1-P
	 */

#define SWS_HSI	0
#define SWS_CSI	1
#define SWS_HSE	2
#define SWS_PLL1_P	3

	/* select PLL1_P as the sys clock.
	 * this runs CPU1 at 480 Mhz.
	 */
	rp->cfgr = SWS_PLL1_P;
}

static void
rcc_pll_240 ( void )
{
	struct rcc *rp = (struct rcc *) RCC_BASE;
	int tmo;
	u32 val;

#define CR_HSI48_ON		BIT(12)
#define CR_HSI48_RDY	BIT(13)

#define CR_HSE_ON	BIT(16)
#define CR_HSE_RDY	BIT(17)

#define CR_PLL1_ON	BIT(24)
#define CR_PLL1_RDY	BIT(25)
#define CR_PLL2_ON	BIT(26)
#define CR_PLL2_RDY	BIT(27)
#define CR_PLL3_ON	BIT(28)
#define CR_PLL3_RDY	BIT(29)


#define	CR_HSIDIV_MASK	(0x3<<3)
#define	CR_HSIDIV1	(0x0<<3)
#define	CR_HSIDIV2	(0x1<<3)
#define	CR_HSIDIV4	(0x2<<3)
#define	CR_HSIDIV8	(0x3<<3)

	/* No MCO, make HSI the system clock.
	 * Do this before killing PLL
	 */
	rp->cfgr = 0;

	turn_off ( CR_PLL1_ON, CR_PLL1_RDY );
	turn_off ( CR_PLL2_ON, CR_PLL2_RDY );
	turn_off ( CR_PLL3_ON, CR_PLL3_RDY );

	/* The base HSI clock is 64 Mhz.
	 * divide by 4 to get 16 Mhz
	 * Must turn off PLL before doing this.
	 */
	// modreg ( rp->cr, CR_HSIDIV_MASK, CR_HSIDIV4 );
	modreg ( rp->cr, CR_HSIDIV_MASK, CR_HSIDIV8 );
	// modreg ( rp->cr, CR_HSIDIV_MASK, CR_HSIDIV1 );

	gdb0 = 0xdeadbeef;
	gdb1 = rp->cr;

	/* Turn on HSE */
	/* We see this count down from 40000 to 38834
	 */
	turn_on ( CR_HSE_ON, CR_HSE_RDY );

	/* Why not?  Turn on HSI48 also */
	/* We see this count down from 40000 to 39981
	 */
	turn_on ( CR_HSI48_ON, CR_HSI48_RDY );

	gdb2 = rp->cr;

	/* All PLL must be off to be able to mess with
	 * the pllckselr.
	 */

	/* All 3 PLL get the same clock,
	 * we select HSE
	 */
#define PLLCK_SEL_MASK	0x3
#define PLLCK_SEL_HSE	0x2
	modreg ( rp->pllckselr, PLLCK_SEL_MASK, PLLCK_SEL_HSE );

#define PLLCK_DIV1_MASK	(0x3f << 4)
#define PLLCK_DIV2_MASK	(0x3f << 12)
#define PLLCK_DIV3_MASK	(0x3f << 20)

#ifdef NUCLEO
#define PLLCK_DIV1_1	(1 << 4)
#define PLLCK_DIV2_1	(1 << 12)
#define PLLCK_DIV3_1	(1 << 20)

	/* All 3 PLL get HSE/1 (i.e. 8 Mhz)  */
	modreg ( rp->pllckselr, PLLCK_DIV1_MASK, PLLCK_DIV1_1 );
	modreg ( rp->pllckselr, PLLCK_DIV2_MASK, PLLCK_DIV2_1 );
	modreg ( rp->pllckselr, PLLCK_DIV3_MASK, PLLCK_DIV3_1 );
#else
#define PLLCK_DIV1_5	(5 << 4)
#define PLLCK_DIV2_5	(5 << 12)
#define PLLCK_DIV3_5	(5 << 20)

	/* All 3 PLL get HSE/5 (i.e. 5 Mhz)  */
	modreg ( rp->pllckselr, PLLCK_DIV1_MASK, PLLCK_DIV1_5 );
	modreg ( rp->pllckselr, PLLCK_DIV2_MASK, PLLCK_DIV2_5 );
	modreg ( rp->pllckselr, PLLCK_DIV3_MASK, PLLCK_DIV3_5 );
#endif

	gdb3 = rp->pllckselr;

#define PLLDIV_R_SHIFT	24
#define PLLDIV_Q_SHIFT	16
#define PLLDIV_P_SHIFT	9

/* The following is a final divide by 2 on the output */

#ifdef NUCLEO
#define PLLDIV		1	// divide by 2
#define PLLMUL		59	// 8 * 60 = 480
#else
#define PLLDIV		1	// divide by 2
#define PLLMUL		95	// 5 * 96 = 480
#endif

	/* Configure those PLL
	 * 5 Mhz * 96 - 480 Mhz
	 * 8 Mhz * 60 - 480 Mhz
	 */
	rp->pll1divr =
	    (PLLDIV << PLLDIV_R_SHIFT) |
	    (PLLDIV << PLLDIV_Q_SHIFT) |
	    (PLLDIV << PLLDIV_P_SHIFT) | PLLMUL;
	rp->pll1fracr = 0;

	rp->pll2divr =
	    (PLLDIV << PLLDIV_R_SHIFT) |
	    (PLLDIV << PLLDIV_Q_SHIFT) |
	    (PLLDIV << PLLDIV_P_SHIFT) | PLLMUL;
	rp->pll2fracr = 0;

	rp->pll3divr =
	    (PLLDIV << PLLDIV_R_SHIFT) |
	    (PLLDIV << PLLDIV_Q_SHIFT) |
	    (PLLDIV << PLLDIV_P_SHIFT) | PLLMUL;
	rp->pll3fracr = 0;

	/* the pllcfgr has VCO control fields along with
	 * enables for the various divider outputs
	 */
#define PLL1_FRACEN	BIT(0)
#define PLL2_FRACEN	BIT(4)
#define PLL3_FRACEN	BIT(8)

/* 0 is wide (192-836), 1 is medium (150-420)
 */
#define PLL1_WIDE	BIT(1)
#define PLL2_WIDE	BIT(5)
#define PLL3_WIDE	BIT(9)

#define PLL1_R_12	0
#define PLL1_R_24	(1<<2)
#define PLL1_R_48	(2<<2)
#define PLL1_R_816	(3<<2)

#define PLL2_R_12	0
#define PLL2_R_24	(1<<6)
#define PLL2_R_48	(2<<6)
#define PLL2_R_816	(3<<6)

#define PLL3_R_12	0
#define PLL3_R_24	(1<<10)
#define PLL3_R_48	(2<<10)
#define PLL3_R_816	(3<<10)

#define PLL1_PEN	BIT(16)
#define PLL1_QEN	BIT(17)
#define PLL1_REN	BIT(18)
#define PLL2_PEN	BIT(19)
#define PLL2_QEN	BIT(20)
#define PLL2_REN	BIT(21)
#define PLL3_PEN	BIT(22)
#define PLL3_QEN	BIT(23)
#define PLL3_REN	BIT(24)
#define PLL_PQR_ALL	(0x1f<<16)

/* We feed them all 5 or 8 Mhz and ask for 480 out */
#define PLL_VCO		PLL1_R_48 | PLL2_R_48 | PLL3_R_48
#define PLL_VCO_MASK	0x0fff

	modreg ( rp->pllcfgr, PLL_VCO_MASK, PLL_VCO );
	rp->pllcfgr |= PLL_PQR_ALL;

	/* Turn them on
	 */
	turn_on ( CR_PLL1_ON, CR_PLL1_RDY );
	turn_on ( CR_PLL2_ON, CR_PLL2_RDY );
	turn_on ( CR_PLL3_ON, CR_PLL3_RDY );

/* Now the sys_clk is running at 480 and we want
 * to set up the various bus clocks.
 */

	/* D1CPRE is the first thing that divides the sys clock.
	 * we leave it at 0 so we keep the full 480 for CPU1.
	 * HPRE will then divide by 2 so we get 240 for CPU2.
	 * We set D1PRE to 4 to send 60 Mhz to APB3/AHB3.
	 */
#define D1_CPRE_1	0
#define D1_HPRE_2	8
#define D1_PRE_4	(5<<4)
	/* Set HPRE to 2 so we get a 240 Mhz clock for CPU2
	 * as well as the various unscaled peripheral clocks
	 * This is APB3
	 */
	rp->d1cfgr = D1_HPRE_2 | D1_PRE_4;

	/* For the D2 domain we also divide the peripheral clocks
	 * down by 4 to get 60 Mhz.
	 * This is APB2 and APB1
	 */
#define D2_PRE1_4	(5<<4)
#define D2_PRE2_4	(5<<8)
	rp->d2cfgr = D2_PRE1_4 | D2_PRE2_4;

	/* For the D3 domain we also divide the peripheral clock
	 * down by 4 to get 60 Mhz.
	 * this is APB4
	 */
#define D3_PRE_4	(5<<4)
	rp->d3cfgr = D3_PRE_4;

	/* Switch the system clock from HSI to PLL1-P
	 */

#define SWS_HSI	0
#define SWS_CSI	1
#define SWS_HSE	2
#define SWS_PLL1_P	3

	/* select PLL1_P as the sys clock.
	 * this runs CPU1 at 480 Mhz.
	 */
	rp->cfgr = SWS_PLL1_P;
}

// #define CLOCK_64
#define CLOCK_240
// #define CLOCK_480

void
rcc_init ( void )
{
	rcc_enables ();
	pwr_setup ();

	// rcc_show ();

#ifdef CLOCK_240
	rcc_pll_240 ();
#endif
#ifdef CLOCK_480
	rcc_pll_480 ();
#endif
}

#ifdef CLOCK_240
#define PCLK1           30000000
#define PCLK2           30000000
#define CPU_HZ          240000000
#endif

#ifdef CLOCK_480
#define PCLK1           60000000
#define PCLK2           60000000
#define CPU_HZ          480000000
#endif

#ifdef CLOCK_64
/* This is how things run "out of the box"
 *
 * Our first guess was 96 Mhz, but
 *  stopwatch indicated more like 68 Mhz.
 * Searching says 64 Mhz "out of the box" on
 * all the H7 parts.  It is running on the internal
 * HSI RC oscillator, with all the PLL disabled/bypassed.
 */
#define PCLK1           64000000
#define PCLK2           64000000
#define CPU_HZ          64000000
#endif

int
get_cpu_hz ( void )
{
    return CPU_HZ;
}

/* The uart driver needs this */
int
get_pclk2_hz ( void )
{
    return PCLK2;
}

// ==================================================================
// ==================================================================

struct pwr {
    vu32    cr1;
    vu32    csr1;
    vu32    cr2;
    vu32    cr3;
    vu32    cpu1cr; /* 0x10 */
    vu32    cpu2cr;
    vu32    d3cr;
    u32 _pad0;
    vu32    wkupcr; /* 0x20 */
    vu32    wkupfr;
    vu32    wkupepr;
};

#define PWR_BASE	(struct pwr *) 0x58024800

/* default (reset) state of cr3 is 0x6
 * which enables both SMPS and LDO
 */
#define CR3_SDEN    BIT(2)
#define CR3_LDOEN   BIT(1)
#define CR3_BYPASS  BIT(0)

/* In some sources the d3cr register is called the srdcr register
 */
#define VOS_POS	14
#define VOS_MASK	(0x3<<VOS_POS)
#define VOS_READY	BIT(13)

#ifdef notdef
/* 2. CONFIGURE VOLTAGE REGULATOR TO VOS1 FIRST
       (VOS0 requires stepping up through VOS1 or being in a specific power state) */
    PWR->CR3 &= ~PWR_CR3_BYPASS; // Ensure internal regulator is active

    // Set VOS bits to 0x3 (VOS1) in the SRDCR (formerly D3CR) register
    PWR->SRDCR = (PWR->SRDCR & ~PWR_SRDCR_VOS) | (3 << PWR_SRDCR_VOS_Pos);
    while (!(PWR->SRDCR & PWR_SRDCR_VOSRDY)); // Wait for voltage to stabilize
#endif

/* To run the chip at 480 Mhz, we need to do some things with
 * regard to power control and voltages.
 */
static void
pwr_setup ( void )
{
	struct pwr *pp = PWR_BASE;

	/* This is probably already clear, but we make sure */
	pp->cr3 &= ~CR3_BYPASS;

	/* The the VOS value to "Scale 1" (i.e. set the value "3") */
	pp->d3cr &= ~VOS_MASK;
	pp->d3cr |= 3<<VOS_POS;

	while ( ! (pp->d3cr & VOS_READY) )
		;

    /* smps is already enabled */
}

/* THE END */
