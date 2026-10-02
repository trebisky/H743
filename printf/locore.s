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
.word   systick_handler /* 15 SysTick */

@ Now our list of IRQ
.word   spin        /* IRQ 0 -- WWDG1 */
.word   spin        /* IRQ 1 -- PVD_PVM */
.word   spin        /* IRQ 2 -- RTC, tamper, ts */
.word   spin        /* IRQ 3 -- RTC_WKUP  */
.word   spin        /* IRQ 4 -- FLASH  */
.word   spin        /* IRQ 5 -- RCC  */
.word   spin        /* IRQ 6 -- EXTI line 0  */
.word   spin        /* IRQ 7 -- EXTI line 1  */
.word   spin        /* IRQ 8 -- EXTI line 2  */
.word   spin        /* IRQ 9 -- EXTI line 3  */
.word   spin        /* IRQ 10 - EXTI line 4  */

.word   spin        /* IRQ 11 - DMA1 Stream 0  */
.word   spin        /* IRQ 12 - DMA1 Stream 1  */
.word   spin        /* IRQ 13 - DMA1 Stream 2  */
.word   spin        /* IRQ 14 - DMA1 Stream 3  */
.word   spin        /* IRQ 15 - DMA1 Stream 4  */
.word   spin        /* IRQ 16 - DMA1 Stream 5  */
.word   spin        /* IRQ 17 - DMA1 Stream 6  */
.word   spin        /* IRQ 18 - ADC 1 and 2  */
.word   spin        /* IRQ 19 - FDCAN1 int 0  */
.word   spin        /* IRQ 20 - FDCAN2 int 0  */
.word   spin        /* IRQ 21 - FDCAN1 int 1  */
.word   spin        /* IRQ 22 - FDCAN2 int 1  */
.word   spin        /* IRQ 23 - EXTI line 9:5  */
.word   spin        /* IRQ 24 - TIM1 break  */
.word   spin        /* IRQ 25 - TIM1 update  */
.word   spin        /* IRQ 26 - TIM1 trigger, commutation  */
.word   spin        /* IRQ 27 - TIM1 capture, compare  */
.word   spin        /* IRQ 28 - TIM2  */
.word   spin        /* IRQ 29 - TIM3  */
.word   spin        /* IRQ 30 - TIM4  */
.word   spin        /* IRQ 31 - i2c1 event */
.word   spin        /* IRQ 31 - x */
		    /* On to IRQ 149 */
.section .text

.thumb_func
spin:   b spin

.thumb_func
_reset:
    bl stm_init
    b .

/* THE END */
