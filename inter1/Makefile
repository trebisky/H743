# Makefile for second blink demo
#
# First code for the H743
# based on my makefile for F411 bare metal
# Tom Trebisky  5-2-2025  9-30-2026

TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as

# In truth the implicit fn warnings are good, but for the purpose of
#  these demos, I can't be busy to set up prototypes as I should.
#COPTS = -mcpu=cortex-m7 -mthumb -Wno-implicit-function-declaration -Os
COPTS = -mcpu=cortex-m7 -mthumb -Wno-implicit-function-declaration


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

OBJS = main.o serial.o gpio.o rcc.o locore.o

all: inter1.elf inter1.dump

inter1.dump:	inter1.elf
	$(DUMP) inter1.elf >inter1.dump

inter1.elf: 	$(OBJS)
	$(LD) $(LDOPTS) -T h743.lds -o inter1.elf $(OBJS)

locore.o:	locore.s
	$(AS) locore.s -o locore.o

serial.o:	serial.c
	$(CC) -c serial.c

gpio.o:	gpio.c
	$(CC) -c gpio.c

#OCDCFG = -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32h7x.cfg
#OCDCFG = -f interface/stlink-v2.cfg -f target/stm32h7x.cfg
OCDCFG = -f interface/stlink.cfg -f target/stm32h7x.cfg

ocd:
	openocd $(OCDCFG)

flash:  inter1.elf
	openocd $(OCDCFG) -c "program inter1.elf verify reset exit"

gdb:
	$(GDB) --eval-command="target remote localhost:3333" inter1.elf

gdbtui:
	$(GDB) -tui --eval-command="target remote localhost:3333" inter1.elf

clean:
	rm -f *.o inter1.elf inter1.dump
