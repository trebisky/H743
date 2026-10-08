/* main.c
 *
 * (c) Tom Trebisky  11-20-2020 (F411)
 * (c) Tom Trebisky  5-2-2025 (H743)
 * (c) Tom Trebisky  10-1-2026 (H743)
 *
 * Demo to test the hardware random number generator
 *  (RNG)
 * This was taken from the print demo.
 *
 * The random number code works fine on the H747, but here I get
 * a clock too slow error.  However, I have done almost nothing
 * to set up the clocks, so that is not that surprising.
 */

#include "protos.h"

/* int and long are both 4 bytes on 32 bit arm,
 * but are 4 and 8 on 64 bit, so beware.
 */
typedef volatile unsigned int vu32;
typedef unsigned int u32;

static int n_tick;

static void
run_loop ( void )
{
	// show_n ( "Tick:", ++n_tick );
	//printf ( "Tick: %d\n", ++n_tick );
	led_next ();
}

static int mycount;

/* This gets called at 1000 Hz.
 * We flip the LED every 0.5 seconds
 *  to get a 1 Hz blink rate.
 */
#define SYSDIV 500

static void
systick_fn ( void )
{
	// console_puts ( "-" );

	mycount++;
	if ( mycount >= SYSDIV ) {
		// console_puts ( "$" );
		run_loop ();
		mycount = 0;
	}
}

/* The idea here is that this is an "idle loop", i.e. a place
 * for the processor to sit and wait for interrupts.
 * This was originally just a hard spin loop.
 * Now I use "wfe" - there is also "wfi" and people talk
 * about masking interrupts before launching one of these,
 * which sounds like exactly the wrong thing to do,
 * but if you let the processor come out of wfe and then
 * immediately unmask interrupts, maybe that is the thing to
 * do and will let the processor enter a deeper sleep mode.
 * It certainly does work.
 */
void
idle ( void )
{
	printf ( "Idle (spinning with wfi)\n" );
    for ( ;; ) {
        /*
        irq_disable ();
        asm volatile( "wfe" );
        irq_enable ();
        */
        asm volatile( "wfi" );
    }
}

void
startup ( void )
{
	int i;
	u32 val;

	printf ( "H743 demo starting\n" );
	rcc_show ();

	systick_hookup ( systick_fn );

	// for ( i=0; i<2; i++ ) {
	for ( i=0; i<10; i++ ) {
		val = random_next ();
		printf ( "Random: %d\n", val );
	}

#ifdef notdef
	for ( ;; ) {
		printf ( "AB" );
	}
	printf ( "Encyclopedia\n" );
#endif

	val = get_cpu_hz () / (1000 * 1000);
	printf ( "Cpu running at %d Hz\n", val );

	/* spin */
	idle ();
}

/* THE END */
