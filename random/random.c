/* random.c
 * (c) Tom Trebisky  1-22-2024
 *
 * driver for the STM32H747 random number hardware
 *
 * Interestingly, the H747 includes hardware that generates
 * 32 bit true random numbers
 * 
 * see RM0399 page 1417 (section 36)
 * (also page 141 for addresses)
 */

#include "protos.h"

#define RAND_BASE	(struct random *) 0x48021800

struct random {
	vu32	cr;		/* 00 */
	vu32	status;		/* 04 */
	vu32	data;		/* 08 */
};

#define CR_ENA	4

#define ST_READY	1

void
random_init ( void )
{
	struct random *rp = RAND_BASE;

	rp->cr = CR_ENA;
}

u32
random_next ( void )
{
	struct random *rp = RAND_BASE;
	int tmo = 10000;

	while ( tmo-- )
	    if ( rp->status & ST_READY )
		return rp->data;

	printf ( "Random: %08x %08x\n", rp->cr, rp->status );
	return 0;
}

/* THE END */
