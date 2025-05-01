# 3-2-2025

#DUMP = arm-none-eabi-objdump -d -z
DUMP = arm-none-eabi-objdump --disassembler-options=force-thumb -d -z

OCDCFG = -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32h7x.cfg

all: wrap imsizer

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

#arm-none-eabi-objdump -d hydra.elf >hydra.dump
demo.dis:	demo.elf
	$(DUMP) demo.elf >demo.dis

demo.elf:	demo.bin
	./wrap demo.bin demo.elf

clean:
	rm -f wrap imsizer
	rm -f demo.dis demo.elf

# THE END

