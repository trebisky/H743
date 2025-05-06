# 3-2-2025

#DUMP = arm-none-eabi-objdump -d -z
DUMP = arm-none-eabi-objdump --disassembler-options=force-thumb -d -z

OCDCFG = -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32h7x.cfg

all: wrap imsizer longs word

ocd:
	openocd $(OCDCFG)

fetch:
	#openocd $(OCDCFG) -c "dump_image fetch.bin 0x08000000 0x200000"
	#openocd $(OCDCFG) -c "flash dump_image fetch.bin 0x08000000 0x30000"
	openocd $(OCDCFG) -c "init" -c "reset_halt"

#openocd -f board/stm3241g_eval_stlink.cfg \
#\
#-c "init" \
#-c "reset halt" \
#-c "dump_image dump/image.bin 0x08000000 0x100000" \
#-c "shutdown" \

# 0x320 = 800
#file1.jpg:	demo.bin
#	dd if=demo.bin of=file1.jpg bs=800 skip=1
file1.jpg:	demo.bin
	dd if=demo.bin of=zzz bs=800 skip=1
	dd if=zzz of=file1.jpg bs=40620 count=1
	rm zzz

# 0x293e8 = 168936
# 0x29de4 = 171492
file2.jpg:	demo.bin
	dd if=demo.bin of=zzz bs=168936 skip=1
	dd if=zzz of=file2.jpg bs=2556 count=1
	rm zzz

demo.dis:	demo.elf
	$(DUMP) demo.elf >demo.dis

demo.elf:	demo.bin
	./wrap demo.bin demo.elf

clean:
	rm -f wrap imsizer
	rm -f demo.dis demo.elf

# THE END

