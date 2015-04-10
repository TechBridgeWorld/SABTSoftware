################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../learn_letter_mode.c \
../SABT_MainUnit.c \
../USART_Keypad.c \
../USART_PC.c \
../VS1053.c \
../SPI.c \
../SD_routines.c \
../FAT32.c \
../UI_Handle.c \
../MD1.c \
../MD2.c \
../MD3.c \
../PC_Handle.c \
../MD4.c \
../MD5.c \
../MD6.c \
../audio.c \
../script_common.c \
../MD7.c \
../common.c \
../MD8.c \
../MD9.c \
../io.c \
../glyph.c \
../MD10.c \
../MD12.c \
../MD11.c \
../letter_globals.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
learn_letter_mode.o \
SABT_MainUnit.o \
USART_Keypad.o \
USART_PC.o \
VS1053.o \
SPI.o \
SD_routines.o \
FAT32.o \
UI_Handle.o \
MD1.o \
MD2.o \
MD3.o \
PC_Handle.o \
MD4.o \
MD5.o \
MD6.o \
audio.o \
script_common.o \
MD7.o \
common.o \
MD8.o \
MD9.o \
io.o \
glyph.o \
MD10.o \
MD12.o \
MD11.o \
letter_globals.o

OBJS_AS_ARGS +=  \
learn_letter_mode.o \
SABT_MainUnit.o \
USART_Keypad.o \
USART_PC.o \
VS1053.o \
SPI.o \
SD_routines.o \
FAT32.o \
UI_Handle.o \
MD1.o \
MD2.o \
MD3.o \
PC_Handle.o \
MD4.o \
MD5.o \
MD6.o \
audio.o \
script_common.o \
MD7.o \
common.o \
MD8.o \
MD9.o \
io.o \
glyph.o \
MD10.o \
MD12.o \
MD11.o \
letter_globals.o

C_DEPS +=  \
learn_letter_mode.d \
SABT_MainUnit.d \
USART_Keypad.d \
USART_PC.d \
VS1053.d \
SPI.d \
SD_routines.d \
FAT32.d \
UI_Handle.d \
MD1.d \
MD2.d \
MD3.d \
PC_Handle.d \
MD4.d \
MD5.d \
MD6.d \
audio.d \
script_common.d \
MD7.d \
common.d \
MD8.d \
MD9.d \
io.d \
glyph.d \
MD10.d \
MD12.d \
MD11.d \
letter_globals.d

C_DEPS_AS_ARGS +=  \
learn_letter_mode.d \
SABT_MainUnit.d \
USART_Keypad.d \
USART_PC.d \
VS1053.d \
SPI.d \
SD_routines.d \
FAT32.d \
UI_Handle.d \
MD1.d \
MD2.d \
MD3.d \
PC_Handle.d \
MD4.d \
MD5.d \
MD6.d \
audio.d \
script_common.d \
MD7.d \
common.d \
MD8.d \
MD9.d \
io.d \
glyph.d \
MD10.d \
MD12.d \
MD11.d \
letter_globals.d

OUTPUT_FILE_PATH +=SABT_MainUnit.elf

OUTPUT_FILE_PATH_AS_ARGS +=SABT_MainUnit.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler

























































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DF_CPU=8000000UL  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega1284p -c -gdwarf-2 -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="SABT_MainUnit.map" -Wl,--start-group  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega1284p  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "SABT_MainUnit.elf" "SABT_MainUnit.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "SABT_MainUnit.elf" "SABT_MainUnit.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "SABT_MainUnit.elf" > "SABT_MainUnit.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "SABT_MainUnit.elf" "SABT_MainUnit.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "SABT_MainUnit.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SABT_MainUnit.elf" "SABT_MainUnit.a" "SABT_MainUnit.hex" "SABT_MainUnit.lss" "SABT_MainUnit.eep" "SABT_MainUnit.map" "SABT_MainUnit.srec" "SABT_MainUnit.usersignatures"
	