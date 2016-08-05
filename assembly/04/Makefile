CROSS_COMPILE ?= arm-none-eabi-

.PHONY: all

all: example.bin example.list

example.o: example.s
	${CROSS_COMPILE}as -mcpu=cortex-m3 -mthumb $< -o $@

example.out: example.o
	${CROSS_COMPILE}ld -Ttext 0x0 -Tdata 0x20000000 -o $@ $<

example.bin: example.out
	${CROSS_COMPILE}objcopy -Obinary -R .data $< $@

example.list: example.out
	${CROSS_COMPILE}objdump -S $< > $@

clean:
	rm -f *.o *.out *.bin *.list
