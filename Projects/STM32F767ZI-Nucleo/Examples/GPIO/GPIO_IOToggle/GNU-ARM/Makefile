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

DEFS += -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_STM32F7XX_NUCLEO_144

INCS += -I../Inc \
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

CFLAGS += -mthumb -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -Wall
LFLAGS += -mcpu=cortex-m7 -mthumb
#-mfloat-abi=hard 
all:IO_Toggle.bin
clean:
	rm -rf $(OBJS) IO_Toggle.bin IO_Toggle.elf
IO_Toggle.bin:IO_Toggle.elf
	arm-none-eabi-objcopy -O binary -S $< $@
IO_Toggle.elf:$(OBJS)
	arm-none-eabi-gcc $(LFALGS) $^ -TSTM32F767ZITx_FLASH.ld -o $@
	arm-none-eabi-size $@
%.o:%.S
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
%.o:%.c
	arm-none-eabi-gcc $(CFLAGS) $(DEFS) $(INCS) -c $< -o $@

