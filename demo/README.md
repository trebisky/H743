H743 project -- demo firmware

Adventures with a STM32H743 Nucleo board

5-1-2025

The board came with some kind of demo or diagnostic firmware
on the board.  The board has 2M of flash, but the demo is
in 192K (actually less) of this.

The game here is to read it out, disassemble it, and study it.
The code is in ARM Thumb instructions, which is a new thing for
me, so just that makes the study worthwhile.

The main file of interest here is demo.txt if you are interested
in my final annotated and augmented disassembly.

The rest of the files here are tools I use to analyze and generate
that disassembly, and those may be of interest to some people as
well.
