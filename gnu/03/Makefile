CROSS_COMPILE ?= arm-none-eabi-

.PHONY: all

all: example.bin example.list

examplea.o: examplea.s
	${CROSS_COMPILE}as -mcpu=cortex-m3 -mthumb $< -o $@

exampleb.o: exampleb.s
	${CROSS_COMPILE}as -mcpu=cortex-m3 -mthumb $< -o $@

example.out: examplea.o exampleb.o
	${CROSS_COMPILE}ld -Ttext 0x0 -o $@ $^

example.bin: example.out
	${CROSS_COMPILE}objcopy -Obinary $< $@

example.list: example.out
	${CROSS_COMPILE}objdump -S $< > $@

clean:
	rm -f *.o *.out *.bin *.list
