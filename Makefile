# Modern C++ and C Mixed Makefile for STM32F446RE
TARGET = MyNucleof446re

MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
LD = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

OPT = -O0 -g3
C_INCLUDES = \
-IInc \
-IInc/CPP_MCAL \
-IInc/CPP_RTOS

C_DEFS = -DSTM32F446xx

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Wextra -fdata-sections -ffunction-sections
CXXFLAGS = $(CFLAGS) -std=c++17 -fno-exceptions -fno-rtti -fstrict-volatile-bitfields -Wno-volatile

LDFLAGS = $(MCU) -specs=nano.specs -specs=nosys.specs -TSTM32F446RETX_FLASH.ld -Wl,-Map=$(TARGET).map,--cref -Wl,--gc-sections

C_SOURCES = \
Src/syscalls.c \
Src/sysmem.c

CPP_SOURCES = \
Src/main.cpp \
Src/CPP_MCAL/Stk.cpp \
Src/CPP_MCAL/Exti.cpp \
Src/CPP_RTOS/Rtos.cpp

ASM_SOURCES = \
Startup/startup_stm32f446retx.s

# Object files
OBJS = $(C_SOURCES:.c=.o) $(CPP_SOURCES:.cpp=.o) $(ASM_SOURCES:.s=.o)

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin print_size

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary -S $< $@

print_size: $(TARGET).elf
	$(SIZE) $<

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).map
