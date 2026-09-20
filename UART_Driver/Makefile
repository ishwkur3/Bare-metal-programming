cc = arm-none-eabi-gcc

CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -g -fno-builtin
LDFLAGS = -nostdlib -T stm32_ls.ld *.o -o Bare_Metal_Program.elf -Wl,-Map=Bare_Metal_Program.map

all: Bare_Metal_Program.elf

main.o: main.c uart.h
	$(cc) ${CFLAGS} $< -o $@

stm32f411_startup.o: stm32f411_startup.c
	$(cc) ${CFLAGS} $^ -o $@

uart.o: uart.c uart.h
	$(cc) ${CFLAGS} $< -o $@

Bare_Metal_Program.elf: main.o uart.o stm32f411_startup.o 
	$(cc) $(LDFLAGS)

load:
	C:\Users\kurku\Downloads\xpack-openocd-0.12.0-7-win32-x64\xpack-openocd-0.12.0-7\bin\openocd -f board/st_nucleo_f4.cfg

clean:
	del -f *.o *.elf *.map