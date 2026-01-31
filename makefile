CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCPY = arm-none-eabi-objcopy

CFLAGS = -c -mcpu=cortex-m3
LDFLAGS = -T linker.ld -nostartfiles -nostdlib -lgcc 

OBJS = ./bin/main.o ./bin/reset.o ./bin/GPIO.o ./bin/RCC.o \
		./bin/USART.o

all: firmware.bin


firmware.bin: firmware.elf
	$(OBJCPY) -O binary $< $@

firmware.elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

./bin/main.o: ./main.c
	$(CC) $(CFLAGS) $< -o $@

./bin/reset.o: ./src/reset.c ./include/reset.h
	$(CC) $(CFLAGS) $< -o $@


./bin/RCC.o: ./src/RCC.c ./include/RCC.h
	$(CC) $(CFLAGS) $< -o $@


./bin/GPIO.o: ./src/GPIO.c ./include/GPIO.h
	$(CC) $(CFLAGS) $< -o $@

./bin/USART.o: ./src/USART.c ./include/USART.h
	$(CC) $(CFLAGS) $< -o $@



.PHONY: clean flash dasm

flash-unix: firmware.bin
	st-flash write 0x08000000 --reset

flash-windows: firmware.bin
	ST-LINK_CLI -P $< 0x08000000

dasm: firmware.bin
	arm-none-eabi-objdump -D -b binary -m arm --adjust-vma=0x08000000 $< > dasm.txt 

clean:
	rm -rf  firmware.elf firmware.bin dasm.txt