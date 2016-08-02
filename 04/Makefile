CROSS_COMPILE ?= arm-none-eabi-

.PHONY: all

all: main.bin

main.o: main.c
	${CROSS_COMPILE}gcc -g -mcpu=cortex-m3 -mthumb -nostartfiles -c $< -o $@

main.out: main.o stm32f103c8t6.ld
	${CROSS_COMPILE}ld -T stm32f103c8t6.ld -o $@ $<

main.bin: main.out
	${CROSS_COMPILE}objcopy -j .text -O binary $< $@

clean:
	rm -f *.o *.out *.bin
