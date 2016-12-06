vpath %.c Src/ \
../../Drivers/STM32F7xx_HAL_Driver/Src

vpath %.h Inc/ \
../../Drivers/BSP/Components/Common \
../../Drivers/STM32F7xx_HAL_Driver/Inc \
../../Drivers/CMSIS/ \
../../Drivers/CMSIS/Include \
../../Drivers/CMSIS/Device/Include/

#vpath %.a ../../Drivers/CMSIS/Lib

CROSS_COMPILE    = arm-none-eabi

CC	 	 = $(CROSS_COMPILE)-gcc
#LD               = $(CROSS_COMPILE)-ld -v
OD               = $(CROSS_COMPILE)-objdump
CP               = $(CROSS_COMPILE)-objcopy
SZ               = $(CROSS_COMPILE)-size

Project := AppJump
TGT_ELF := $(Project).elf
TGT_BIN := $(Project).bin

LDFILE := STM32F765IIKx_FLASH.ld

DEFS = -DUSE_HAL_DRIVER -DSTM32F765xx

INCS +=		-IInc/ \
		-I../../Drivers/BSP/Components/Common \
		-I../../Drivers/STM32F7xx_HAL_Driver/Inc \
		-I../../Drivers/CMSIS/ \
		-I../../Drivers/CMSIS/Include \
		-I../../Drivers/CMSIS/Device/Include/

#LIBPATH +=      -L../../Drivers/CMSIS/Lib/
#LIBS +=         -l../../Drivers/CMSIS/Lib/libarm_cortexM7lfdp_math.a

OBJS += startup_stm32f765xx.o
# Src/stm32f7xx_hal_msp.o
OBJS += Src/main.o Src/stm32f7xx_it.o Src/system_stm32f7xx.o
#
OBJS += ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rng.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.o \
        ../../Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o \

DEBUG_FLAGS	?= -ggdb3
FP_FLAGS	?= -mfloat-abi=soft -mfpu=fpv5-sp-d16
#--specs=rdimon.specs   -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group -o test test.c
FLAGS		= -std=gnu99 \
		-Os -g -MMD \
		-Wundef -Wall -Wimplicit-function-declaration \
#                -Wextra -Wredundant-decls \
#                -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
		-Wshadow -fno-builtin -fdata-sections -MD \
		-ffunction-sections \
                --specs=rdimon.specs \
#                -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group \
                -nostartfiles \
                -lnosys \
                -Wl,-gc-sections \
                -Werror $(EXTRAFLAGS)
# $(FP_FLAGS)
FLAGS += -mthumb -mcpu=cortex-m7 $(DEBUG_FLAGS) $(FP_FLAGS)
LFLAGS += -mthumb -mcpu=cortex-m7
#-Wl,--gc-sections 
LDFLAGS := -lgcc -lc -lm -lrdimon

all:$(TGT_BIN)

$(TGT_BIN):$(TGT_ELF)
	@printf "generate binary...\n"
	@$(CP) -O binary -S $< $@
	@printf "sizeof  $(^F):\n"
	@$(SZ) $^
#
$(TGT_ELF):$(OBJS)
	@printf "  CC      $(<F)\n"
	@$(CC) $^ $(LFALGS) -T$(LDFILE) $(LDFLAGS) -o $@
%.o:%.S
	@printf "  CC      $(<F)\n"
	@$(CC) $(FLAGS) -c $< -o $@
# $(LIBPATH) $(LIBS)
%.o:%.c
	@printf "  CC      $(<F)\n"
	@$(CC) $(FLAGS) $(DEFS) $(INCS) -c $< -o $@
dOBJS := $(OBJS:.o=.d)
clean:
	rm -rf $(OBJS) $(dOBJS) $(Project).*

.PHONY: clean all

