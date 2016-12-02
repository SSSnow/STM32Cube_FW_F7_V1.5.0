vpath %.c ../Src \
../../../../../../Drivers/BSP/STM32F7xx_Nucleo_144 \
../../../../../../Drivers/STM32F7xx_HAL_Driver/Src \
../../../../../../Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/

vpath %.h ../Inc \
../../../../../../Drivers/BSP/STM32F7xx_Nucleo_144 \
../../../../../../Drivers/BSP/Components/Common \
../../../../../../Drivers/STM32F7xx_HAL_Driver/Inc \
../../../../../../Drivers/CMSIS/ \
../../../../../../Drivers/CMSIS/Include \
../../../../../../Drivers/CMSIS/Device/ST/STM32F7xx/Include/

export CC	 	 = arm-none-eabi-gcc
export OBJCOPY		 = arm-none-eabi-objcopy
export SIZE		 = arm-none-eabi-size

DEFS += -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_STM32F7XX_NUCLEO_144

INCS +=		-I../Inc \
		-I../../../../../../Drivers/BSP/STM32F7xx_Nucleo_144 \
		-I../../../../../../Drivers/BSP/Components/Common \
		-I../../../../../../Drivers/STM32F7xx_HAL_Driver/Inc \
		-I../../../../../../Drivers/CMSIS/ \
		-I../../../../../../Drivers/CMSIS/Include \
		-I../../../../../../Drivers/CMSIS/Device/ST/STM32F7xx/Include/

OBJS += startup_stm32f767xx.o
OBJS += ../Src/main.o ../Src/stm32f7xx_it.o ../Src/system_stm32f7xx.o
OBJS += ../../../../../../Drivers/BSP/STM32F7xx_Nucleo_144/stm32f7xx_nucleo_144.o
OBJS += ../../../../../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o \
        ../../../../../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o \
        ../../../../../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o \
        ../../../../../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o \
        ../../../../../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o \
        ../../../../../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o
#OBJS += ../../../../../../Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.o

DEBUG_FLAGS	?= -ggdb3
FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv5-sp-d16

export FLAGS		 = -std=gnu99 \
			   -Os \
			   -g \
			   -Wundef \
			   -Wall \
			   -fno-builtin \
			   -ffunction-sections \
			   -nostartfiles \
			   -lnosys \
			   -Wl,-gc-sections \
			   -Werror \
			   -TSTM32F767ZITx_FLASH.ld

FLAGS += -mthumb -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 $(FP_FLAGS) $(DEBUG_FLAGS)

all:IO_Toggle.bin

clean:
	rm -rf $(OBJS) IO_Toggle.*
.PHONY: clean

IO_Toggle.bin:IO_Toggle.elf
	$(OBJCOPY) -O binary $< $@

IO_Toggle.elf:$(OBJS)
	$(CC) $^ -o $@ $(OBJS) $(FALGS)
	$(SIZE) $@
%.o:%.S
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@
%.o:%.c
	$(CC) $(CFLAGS) $(FLAGS) $(DEFS) $(INCS) -c $< -o $@

