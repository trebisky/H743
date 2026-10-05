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
		    /* On to IRQ 149 */

    /* Taken from startup_stm32h747xx.S in
     * /u1/Projects/STM32/Mbed/mbed-os/targets/..
     *  TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/..
     *  TARGET_STM32H747xI_CM7/TOOLCHAIN_GCC_ARM
     *
     * 150 external interrupt vectors follow.
     */

/* We have to rename this file locore.S to use the cpp */
#ifdef notdef
  .word     I2C1_EV_IRQHandler                /* I2C1 Event                   */
  .word     I2C1_ER_IRQHandler                /* I2C1 Error                   */
  .word     I2C2_EV_IRQHandler                /* I2C2 Event                   */
  .word     I2C2_ER_IRQHandler                /* I2C2 Error                   */
  .word     SPI1_IRQHandler                   /* SPI1                         */
  .word     SPI2_IRQHandler                   /* SPI2                         */
  .word     USART1_IRQHandler                 /* USART1                       */
  .word     USART2_IRQHandler                 /* USART2                       */
  .word     USART3_IRQHandler                 /* USART3                       */
  .word     EXTI15_10_IRQHandler              /* External Line[15:10]s        */
  .word     RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */
  .word     0                                 /* Reserved                     */
  .word     TIM8_BRK_TIM12_IRQHandler         /* TIM8 Break and TIM12         */
  .word     TIM8_UP_TIM13_IRQHandler          /* TIM8 Update and TIM13        */
  .word     TIM8_TRG_COM_TIM14_IRQHandler     /* TIM8 Trigger and Commutation and TIM14 */
  .word     TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */
  .word     DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */
  .word     FMC_IRQHandler                    /* FMC                          */
  .word     SDMMC1_IRQHandler                 /* SDMMC1                       */
  .word     TIM5_IRQHandler                   /* TIM5                         */
  .word     SPI3_IRQHandler                   /* SPI3                         */
  .word     UART4_IRQHandler                  /* UART4                        */
  .word     UART5_IRQHandler                  /* UART5                        */
  .word     TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */
  .word     TIM7_IRQHandler                   /* TIM7                         */
  .word     DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */
  .word     DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */
  .word     DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */
  .word     DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */
  .word     DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */
  .word     ETH_IRQHandler                    /* Ethernet                     */
  .word     ETH_WKUP_IRQHandler               /* Ethernet Wakeup through EXTI line */
  .word     FDCAN_CAL_IRQHandler              /* FDCAN calibration unit interrupt  */
  .word     CM7_SEV_IRQHandler                /* CM7 Send event interrupt for CM4  */
  .word     CM4_SEV_IRQHandler                /* CM4 Send event interrupt for CM7  */
  .word     0                                 /* Reserved                     */
  .word     0                                 /* Reserved                     */
  .word     DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */
  .word     DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */
  .word     DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */
  .word     USART6_IRQHandler                 /* USART6                       */
  .word     I2C3_EV_IRQHandler                /* I2C3 event                   */
  .word     I2C3_ER_IRQHandler                /* I2C3 error                   */
  .word     OTG_HS_EP1_OUT_IRQHandler         /* USB OTG HS End Point 1 Out   */
  .word     OTG_HS_EP1_IN_IRQHandler          /* USB OTG HS End Point 1 In    */
  .word     OTG_HS_WKUP_IRQHandler            /* USB OTG HS Wakeup through EXTI */
  .word     OTG_HS_IRQHandler                 /* USB OTG HS                   */
  .word     DCMI_IRQHandler                   /* DCMI                         */
  .word     0                                 /* Reserved */
  .word     RNG_IRQHandler                    /* Rng                          */
  .word     FPU_IRQHandler                    /* FPU                          */
  .word     UART7_IRQHandler                  /* UART7                        */
  .word     UART8_IRQHandler                  /* UART8                        */
  .word     SPI4_IRQHandler                   /* SPI4                         */
  .word     SPI5_IRQHandler                   /* SPI5                         */
  .word     SPI6_IRQHandler                   /* SPI6                         */
  .word     SAI1_IRQHandler                   /* SAI1                         */
  .word     LTDC_IRQHandler                   /* LTDC                         */
  .word     LTDC_ER_IRQHandler                /* LTDC error                   */
  .word     DMA2D_IRQHandler                  /* DMA2D                        */
  .word     SAI2_IRQHandler                   /* SAI2                         */
  .word     QUADSPI_IRQHandler                /* QUADSPI                      */
  .word     LPTIM1_IRQHandler                 /* LPTIM1                       */
  .word     CEC_IRQHandler                    /* HDMI_CEC                     */
  .word     I2C4_EV_IRQHandler                /* I2C4 Event                   */
  .word     I2C4_ER_IRQHandler                /* I2C4 Error                   */
  .word     SPDIF_RX_IRQHandler               /* SPDIF_RX                     */
  .word     OTG_FS_EP1_OUT_IRQHandler         /* USB OTG FS End Point 1 Out   */
  .word     OTG_FS_EP1_IN_IRQHandler          /* USB OTG FS End Point 1 In    */
  .word     OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI */
  .word     OTG_FS_IRQHandler                 /* USB OTG FS                   */
  .word     DMAMUX1_OVR_IRQHandler            /* DMAMUX1 Overrun interrupt    */
  .word     HRTIM1_Master_IRQHandler          /* HRTIM Master Timer global Interrupt */
  .word     HRTIM1_TIMA_IRQHandler            /* HRTIM Timer A global Interrupt */
  .word     HRTIM1_TIMB_IRQHandler            /* HRTIM Timer B global Interrupt */
  .word     HRTIM1_TIMC_IRQHandler            /* HRTIM Timer C global Interrupt */
  .word     HRTIM1_TIMD_IRQHandler            /* HRTIM Timer D global Interrupt */
  .word     HRTIM1_TIME_IRQHandler            /* HRTIM Timer E global Interrupt */
  .word     HRTIM1_FLT_IRQHandler             /* HRTIM Fault global Interrupt   */
  .word     DFSDM1_FLT0_IRQHandler            /* DFSDM Filter0 Interrupt        */
  .word     DFSDM1_FLT1_IRQHandler            /* DFSDM Filter1 Interrupt        */
  .word     DFSDM1_FLT2_IRQHandler            /* DFSDM Filter2 Interrupt        */
  .word     DFSDM1_FLT3_IRQHandler            /* DFSDM Filter3 Interrupt        */
  .word     SAI3_IRQHandler                   /* SAI3 global Interrupt          */
  .word     SWPMI1_IRQHandler                 /* Serial Wire Interface 1 global interrupt */
  .word     TIM15_IRQHandler                  /* TIM15 global Interrupt      */
  .word     TIM16_IRQHandler                  /* TIM16 global Interrupt      */
  .word     TIM17_IRQHandler                  /* TIM17 global Interrupt      */
  .word     MDIOS_WKUP_IRQHandler             /* MDIOS Wakeup  Interrupt     */
  .word     MDIOS_IRQHandler                  /* MDIOS global Interrupt      */
  .word     JPEG_IRQHandler                   /* JPEG global Interrupt       */
  .word     MDMA_IRQHandler                   /* MDMA global Interrupt       */
  .word     DSI_IRQHandler                    /* DSI global Interrupt        */
  .word     SDMMC2_IRQHandler                 /* SDMMC2 global Interrupt     */
  .word     HSEM1_IRQHandler                  /* HSEM1 global Interrupt      */
  .word     HSEM2_IRQHandler                  /* HSEM1 global Interrupt      */
  .word     ADC3_IRQHandler                   /* ADC3 global Interrupt       */
  .word     DMAMUX2_OVR_IRQHandler            /* DMAMUX Overrun interrupt    */
  .word     BDMA_Channel0_IRQHandler          /* BDMA Channel 0 global Interrupt */
  .word     BDMA_Channel1_IRQHandler          /* BDMA Channel 1 global Interrupt */
  .word     BDMA_Channel2_IRQHandler          /* BDMA Channel 2 global Interrupt */
  .word     BDMA_Channel3_IRQHandler          /* BDMA Channel 3 global Interrupt */
  .word     BDMA_Channel4_IRQHandler          /* BDMA Channel 4 global Interrupt */
  .word     BDMA_Channel5_IRQHandler          /* BDMA Channel 5 global Interrupt */
  .word     BDMA_Channel6_IRQHandler          /* BDMA Channel 6 global Interrupt */
  .word     BDMA_Channel7_IRQHandler          /* BDMA Channel 7 global Interrupt */
  .word     COMP1_IRQHandler                  /* COMP1 global Interrupt     */
  .word     LPTIM2_IRQHandler                 /* LP TIM2 global interrupt   */
  .word     LPTIM3_IRQHandler                 /* LP TIM3 global interrupt   */
  .word     LPTIM4_IRQHandler                 /* LP TIM4 global interrupt   */
  .word     LPTIM5_IRQHandler                 /* LP TIM5 global interrupt   */
  .word     LPUART1_IRQHandler                /* LP UART1 interrupt         */
  .word     WWDG_RST_IRQHandler               /* Window Watchdog reset interrupt (exti_d2_wwdg_it, exti_d1_wwdg_it) */
  .word     CRS_IRQHandler                    /* Clock Recovery Global Interrupt */
  .word     ECC_IRQHandler                    /* ECC diagnostic Global Interrupt */
  .word     SAI4_IRQHandler                   /* SAI4 global interrupt      */
  .word     0                                 /* Reserved                   */
  .word     HOLD_CORE_IRQHandler              /* Hold core interrupt        */
  .word     WAKEUP_PIN_IRQHandler             /* Interrupt for all 6 wake-up pins */
#endif

.section .text

.thumb_func
spin:   b spin

.thumb_func
_reset:
    bl stm_init
    b .

/* THE END */
