CROSS_COMPILE ?= arm-none-eabi-

.PHONY: all

all: blink.bin

blink.o: blink.c
	${CROSS_COMPILE}gcc -mcpu=cortex-m3 -mthumb -nostartfiles -c $< -o $@

blink.out: blink.o simple.ld
	${CROSS_COMPILE}ld -T simple.ld -o $@ $<

blink.bin: blink.out
	${CROSS_COMPILE}objcopy -j .text -O binary $< $@

clean:
	rm -f *.o *.out *.bin
