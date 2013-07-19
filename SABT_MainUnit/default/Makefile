###############################################################################
# Makefile for the project SABT_MainUnit
###############################################################################

## General Flags
PROJECT = SABT_MainUnit
MCU = atmega1284p
TARGET = SABT_MainUnit.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99   -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=SABT_MainUnit.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = SABT_MainUnit.o USART_Keypad.o USART_PC.o VS1053.o SPI.o SD_routines.o FAT32.o UI_Handle.o MD1.o MD2.o MD3.o PC_Handle.o MD4.o MD5.o MD6.o audio.o script_common.o MD7.o common.o MD8.o MD9.o io.o glyph.o MD10.o MD12.o MD11.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) SABT_MainUnit.hex SABT_MainUnit.eep SABT_MainUnit.lss## Compile
SABT_MainUnit.o: ../SABT_MainUnit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

USART_Keypad.o: ../USART_Keypad.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

USART_PC.o: ../USART_PC.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VS1053.o: ../VS1053.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SPI.o: ../SPI.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SD_routines.o: ../SD_routines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

FAT32.o: ../FAT32.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UI_Handle.o: ../UI_Handle.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD1.o: ../MD1.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD2.o: ../MD2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD3.o: ../MD3.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PC_Handle.o: ../PC_Handle.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD4.o: ../MD4.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD5.o: ../MD5.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD6.o: ../MD6.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

audio.o: ../audio.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

script_common.o: ../script_common.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD7.o: ../MD7.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

common.o: ../common.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD8.o: ../MD8.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD9.o: ../MD9.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

io.o: ../io.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

glyph.o: ../glyph.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD10.o: ../MD10.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD12.o: ../MD12.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MD11.o: ../MD11.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) SABT_MainUnit.elf dep/* SABT_MainUnit.hex SABT_MainUnit.eep SABT_MainUnit.lss SABT_MainUnit.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

