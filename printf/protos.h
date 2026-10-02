/* protos.h
 * (c) Tom Trebisky  12-20-2023
 * (c) Tom Trebisky  10-2-2026
 */

typedef unsigned int u32;
typedef volatile unsigned int vu32;

#define BIT(x)  (1<<(x))

void delay ( void );

void led_init ( void );
void led_on ( void );
void led_off ( void );

void rcc_init ( void );

void uart_init ( void );
void uart_putc ( int );
void uart_puts ( char * );

void gpio_init ( void );
// void gpio_uart1_pins ( void );

int printf ( const char *, ... );

// void random_init ( void );
// u32 random_next ( void );

/* THE END */
