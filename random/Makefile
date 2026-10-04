# Makefile for the H743 demos
#
# Originally based on my makefile for F411 bare metal
# Tom Trebisky  5-2-2025  9-30-2026

TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as

# In truth the implicit fn warnings are good, but for the purpose of
#  these demos, I can't be busy to set up prototypes as I should.
#COPTS = -mcpu=cortex-m7 -mthumb -Wno-implicit-function-declaration -Os
#COPTS = -mcpu=cortex-m7 -mthumb -Wno-implicit-function-declaration
# the no-builtin switch is needed, otherwise the compiler tries to
# do clever optimizations like turning simple printf() into puts().
COPTS = -mcpu=cortex-m7 -mthumb -fno-builtin -Wno-implicit-function-declaration

# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb -g
CC = $(TOOLS)-gcc $(COPTS)

# Optimizing for size works fine and reduces code size from .....

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
LDOPTS = --no-warn-rwx-segments

OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

OBJS = init.o main.o prf.o random.o serial.o gpio.o systick.o rcc.o locore.o

all: demo.elf demo.dump

demo.dump:	demo.elf
	$(DUMP) demo.elf >demo.dump

demo.elf: 	$(OBJS)
	$(LD) $(LDOPTS) -T h743.lds -o demo.elf $(OBJS)

locore.o:	locore.s
	$(AS) locore.s -o locore.o

#OCDCFG = -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32h7x.cfg
#OCDCFG = -f interface/stlink-v2.cfg -f target/stm32h7x.cfg
OCDCFG = -f interface/stlink.cfg -f target/stm32h7x.cfg

ocd:
	openocd $(OCDCFG)

# I ran into a weird scenatio where I got the error
#  Warn : target stm32h7x.cpu0 examination failed
#   ** Unable to reset target **
# I fixed it by holding the reset button and typing "make flash",
#  then a subsequent "make flash" worked.

flash:  demo.elf
	#openocd $(OCDCFG) -c "reset_config none" -c "program demo.elf verify reset exit"
	openocd $(OCDCFG) -c "program demo.elf verify reset exit"

gdb:
	$(GDB) --eval-command="target remote localhost:3333" demo.elf

gdbtui:
	$(GDB) -tui --eval-command="target remote localhost:3333" demo.elf

clean:
	rm -f *.o demo.elf demo.dump
