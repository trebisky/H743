/* locore.s
 * Assembler startup file for the STM32H743
 * Tom Trebisky  5-2-2025
 */

# The H743 is a Cortex-M7 processor.
# It is thumb only, like the M3 and M4
# The M4 and M7 support Thumb-2 which has
# some additional instructions.

# The M7 has a lot more interrupt vectors than the M4
# See the TRM (RM0433) page 754 (section 19.1.2)

.section .vectors
.cpu cortex-m7
.thumb

# Notice we put the stack at the top of a 128K region

.word   0x20020000  /* stack top address */
.word   _reset      /* 1 Reset */
.word   spin        /* 2 NMI */
.word   spin        /* 3 Hard Fault */
.word   spin        /* 4 MM Fault */
.word   spin        /* 5 Bus Fault */
.word   spin        /* 6 Usage Fault */
.word   spin        /* 7 RESERVED */
.word   spin        /* 8 RESERVED */
.word   spin        /* 9 RESERVED*/
.word   spin        /* 10 RESERVED */
.word   spin        /* 11 SV call */
.word   spin        /* 12 Debug monitor */
.word   spin        /* 13 RESERVED */
.word   spin        /* 14 PendSV */
.word   spin        /* 15 SysTick */
.word   spin        /* 16 IRQ 0 */
.word   spin        /* 17 IRQ 1 */
.word   spin        /* 18 IRQ 2 */
.word   spin        /* 19 ...   */
		    /* On to IRQ 149 */
.section .text

.thumb_func
spin:   b spin

.thumb_func
_reset:
    bl startup
    b .

/* THE END */
