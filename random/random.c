/* random.c
 * (c) Tom Trebisky  1-22-2024  10-4-2026
 *
 * driver for the STM32H747 random number hardware
 *
 * Interestingly, the H747 includes hardware that generates
 * 32 bit true random numbers
 *
 * Currently this code fails on the H743 with a "clock too slow"
 *  status in the RNG status register.
 * 
 * see RM0399 page 1417 (section 36)
 * (also page 141 for addresses)
 */

#include "protos.h"

/* on H747 and H743 */
#define RAND_BASE	(struct random *) 0x48021800

struct random {
	vu32	cr;			/* 00 */
	vu32	status;		/* 04 */
	vu32	data;		/* 08 */
};

#define CR_ENA	4

#define ST_READY	1
#define ST_CExx		2

void
random_init ( void )
{
	struct random *rp = RAND_BASE;
	int tmo = 10000;

	printf ( "Random (RNG) initialized\n" );
	rp->cr = CR_ENA;

	printf ( "Random status A: %08x\n", rp->status );

	while ( tmo-- )
	    if ( rp->status )
			break;

	printf ( "Random status B: %08x\n", rp->status );

	while ( tmo-- )
	    if ( rp->status & ST_READY )
			break;

	printf ( "Random init: %d %08x %08x %08x\n", tmo, rp->data, rp->cr, rp->status );
}

u32
random_next ( void )
{
	struct random *rp = RAND_BASE;
	int tmo = 10000;

	while ( tmo-- )
	    if ( rp->status & ST_READY )
			return rp->data;

	printf ( "Random timeout: %d %08x %08x\n", tmo, rp->cr, rp->status );
	return 1234;
}

/* THE END */
