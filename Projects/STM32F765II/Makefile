vpath %.c Src/ \
../../Drivers/STM32F7xx_HAL_Driver/Src

vpath %.h Inc/ \
../../Drivers/BSP/Components/Common \
../../Drivers/STM32F7xx_HAL_Driver/Inc \
../../Drivers/CMSIS/ \
../../Drivers/CMSIS/Include \
../../Drivers/CMSIS/Device/Include/

CC	 	 = arm-none-eabi-gcc
OBJCOPY		 = arm-none-eabi-objcopy
SIZE		 = arm-none-eabi-size

DEFS = -DUSE_HAL_DRIVER -DSTM32F765xx

INCS +=		-IInc/ \
		-I../../Drivers/BSP/Components/Common \
		-I../../Drivers/STM32F7xx_HAL_Driver/Inc \
		-I../../Drivers/CMSIS/ \
		-I../../Drivers/CMSIS/Include \
		-I../../Drivers/CMSIS/Device/Include/

OBJS += startup_stm32f765xx.o
OBJS += Src/main.o Src/stm32f7xx_it.o Src/system_stm32f7xx.o
OBJS += ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o

DEBUG_FLAGS	?= -ggdb3
FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv5-sp-d16

FLAGS		= -std=gnu99 \
		-Os \
		-g \
		-Wundef \
		-Wall \
		-fno-builtin \
		-ffunction-sections \
		-nostartfiles \
		-lnosys \
		-Wl,-gc-sections \
		-Werror
# -mfpu=fpv5-sp-d16 $(FP_FLAGS)
FLAGS += -mthumb -mcpu=cortex-m7 -Wall $(DEBUG_FLAGS)
LFLAGS += -mthumb -mcpu=cortex-m7

all:IO_Toggle.bin

IO_Toggle.bin:IO_Toggle.elf
	$(OBJCOPY) -O binary -S $< $@

IO_Toggle.elf:$(OBJS)
	$(CC) $(LFALGS) $^ -TSTM32F765IIKx_FLASH.ld -o $@
	$(SIZE) $@
%.o:%.S
	$(CC) $(FLAGS) -c $< -o $@
%.o:%.c
	$(CC) $(FLAGS) $(DEFS) $(INCS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS) IO_Toggle.*

