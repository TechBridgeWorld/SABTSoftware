###############################################################################
# Makefile for the project UserInterface
###############################################################################

## General Flags
PROJECT = UserInterface
MCU = atmega1284p
TARGET = UserInterface.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                       -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=UserInterface.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = UserInterface.o DigitalIO.o AnalogInputs.o USART_MCU.o Timer.o CommPacket.o ProcessInput.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) UserInterface.hex UserInterface.eep UserInterface.lss## Compile
UserInterface.o: ../UserInterface.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DigitalIO.o: ../DigitalIO.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

AnalogInputs.o: ../AnalogInputs.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

USART_MCU.o: ../USART_MCU.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Timer.o: ../Timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CommPacket.o: ../CommPacket.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ProcessInput.o: ../ProcessInput.c
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
	-rm -rf $(OBJECTS) UserInterface.elf dep/* UserInterface.hex UserInterface.eep UserInterface.lss UserInterface.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

