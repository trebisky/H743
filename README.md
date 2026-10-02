H743 project

Adventures with a STM32H743 Nucleo board

5-1-2025

1. demo - disassembly of demo firmware shipped on the board
2. blink1 - my first bare metal demo -- simplest LED blinker
3. serial - output on seial port, and keep on blinking

9-30-2026

1. blink2 - blink all 3 LED, not just LED1 (and serial output)
2. inter1 - get an interrupt using systick
3. printf - add printf -- and bss initialization

I expect this to be a stepping stone to working with the H755 board
which sports two cores (one M7 and one M4).  I suspect that if the
second M4 core is just ignored, the H755 will work exactly like
the H743.  We shall see.

[my notes on the H743 Nucleo](http://cholla.mmto.org/stm32all/h743)

