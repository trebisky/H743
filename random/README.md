This is the "random" demo

This builds on the printf demo

It is pretty minor.  It just adds random.c to
support the hardware random number generator.

It does NOT work.  Without proper clock setup I get
a "clock too slow" status from the RNG.
I am going to leave this "as is" (i.e. not working)
and move on to work on clock setup.  Then I will
revisit this.

With no extra effort, in the next demo ("cpu") this
just works after I set up clocks in the RCC.

10-4-2026
