/* main.c
 *
 * (c) Tom Trebisky  11-20-2020 (F411)
 * (c) Tom Trebisky  5-2-2025 (H743)
 * (c) Tom Trebisky  10-1-2026 (H743)
 *
 * This was taken from the blink1 demo.
 *
 * Changes to port this from F411 to H743
 *
 * LED on different pin.
 * all base addresses are different.
 * RCC is totally different.
 */

/* int and long are both 4 bytes on 32 bit arm,
 * but are 4 and 8 on 64 bit, so beware.
 */
typedef volatile unsigned int vu32;
typedef unsigned int u32;

static int n_tick;

static void
run_loop ( void )
{
	show_n ( "Tick:", ++n_tick );
	led_next ();
	// check ();
}

static int mycount;

/* This gets called at 1000 Hz.
 */
#define SYSDIV 1000
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
	// string_init ();

	console_puts ( "H743 demo starting\n" );

	systick_hookup ( systick_fn );

	/* spin */
	idle ();
}

/* ================================ old code from previous demos
 * =============================================================
 * =============================================================
 */

#ifdef notdef
/* On the STM32F103, this gave a blink rate of about 2.7 Hz */
/* i.e. the delay time is about 0.2 seconds (200 ms) */
#define FAST	200

#define FASTER	50
#define SLOWER	800
#define SLOWER2	2000

static void
delay ( void )
{
	// volatile int count = 1000 * FAST;
	// volatile unsigned int count = 1000 * SLOWER;
	// gives about 1 hz
	volatile unsigned int count = 1000 * SLOWER2;

	while ( count-- )
	    ;
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
#endif

/* THE END */
