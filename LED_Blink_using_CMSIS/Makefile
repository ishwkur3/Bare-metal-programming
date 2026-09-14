cc = arm-none-eabi-gcc

INCLUDES = -I C:\Users\kurku\Downloads\stm32cubef4-v1-28-0\STM32Cube_FW_F4_V1.28.0\Drivers\CMSIS\Device\ST\STM32F4xx\Include -I C:\Users\kurku\Downloads\stm32cubef4-v1-28-0\STM32Cube_FW_F4_V1.28.0\Drivers\CMSIS\Include

CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -DSTM32F411xE $(INCLUDES)
LDFLAGS = -nostdlib -T stm32_ls.ld *.o -o Bare_Metal_Program.elf -Wl,-Map=Bare_Metal_Program.map

all: Bare_Metal_Program.elf

main.o: main.c
	$(cc) ${CFLAGS} $^ -o $@

stm32f411_startup.o: stm32f411_startup.c
	$(cc) ${CFLAGS} $^ -o $@

Bare_Metal_Program.elf: main.o stm32f411_startup.o
	$(cc) $(LDFLAGS)

load:
	C:\Users\kurku\Downloads\xpack-openocd-0.12.0-7-win32-x64\xpack-openocd-0.12.0-7\bin\openocd -f board/st_nucleo_f4.cfg

clean:
	del -f *.o *.elf *.map