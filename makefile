CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCPY = arm-none-eabi-objcopy

CFLAGS  = -c -mcpu=cortex-m3 -mthumb -std=gnu11
LDFLAGS = -T linker.ld -nostartfiles --specs=nano.specs -mcpu=cortex-m3 -mthumb

OBJS = ./bin/main.o ./bin/reset.o ./bin/GPIO.o ./bin/RCC.o \
       ./bin/USART.o ./bin/SD.o ./bin/SPI.o ./bin/LCD.o \
       ./bin/NVIC.o ./bin/DMA.o ./bin/AFIO.o

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

./bin/SPI.o: ./src/SPI.c ./include/SPI.h
	$(CC) $(CFLAGS) $< -o $@

./bin/SD.o: ./src/SD.c ./include/SD.h
	$(CC) $(CFLAGS) $< -o $@

./bin/LCD.o: ./src/LCD.c ./include/LCD.h
	$(CC) $(CFLAGS) $< -o $@

./bin/NVIC.o: ./src/NVIC.c ./include/NVIC.h
	$(CC) $(CFLAGS) $< -o $@

./bin/DMA.o: ./src/DMA.c ./include/DMA.h
	$(CC) $(CFLAGS) $< -o $@

./bin/AFIO.o: ./src/AFIO.c ./include/AFIO.h
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean flash-unix flash-windows dasm

flash-unix: firmware.bin
	st-flash write $< 0x08000000

flash-windows: firmware.bin
	ST-LINK_CLI -P $< 0x08000000

dasm: firmware.elf
	arm-none-eabi-objdump -D $< > dasm.txt

clean:
	rm -rf firmware.elf firmware.bin dasm.txt ./bin/*.o