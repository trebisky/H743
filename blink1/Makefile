# Makefile for blink demo
#
# First code for the H743
# based on my makefile for F411 bare metal
# Tom Trebisky  5-2-2-25

TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as

# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb -g
CC = $(TOOLS)-gcc -mcpu=cortex-m7 -mthumb -Os

# Optimizing for size works fine and reduces code size from .....

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

#OBJS = locore.o blink.o
OBJS = blink.o locore.o

all: blink.elf blink.dump

blink.dump:	blink.elf
	$(DUMP) blink.elf >blink.dump

blink.elf: 	$(OBJS)
	$(LD) -T blink.lds -o blink.elf $(OBJS)

locore.o:	locore.s
	$(AS) locore.s -o locore.o

blink.o:	blink.c
	$(CC) -c blink.c

OCDCFG = -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32h7x.cfg

flash:  blink.elf
	openocd $(OCDCFG) -c "program blink.elf verify reset exit"

gdb:
	$(GDB) --eval-command="target remote localhost:3333" blink.elf

gdbtui:
	$(GDB) -tui --eval-command="target remote localhost:3333" blink.elf

clean:
	rm -f *.o blink.elf blink.dump
