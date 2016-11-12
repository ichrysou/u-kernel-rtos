CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=arm-gcc-link.ld
OBJ_DIR = obj
BIN_DIR = bin
RTOS_SOURCE_DIR=../../src/uKernel
ARM_SOURCE_DIR=../../src/lpc17xx_lib/source
CORTEX_SOURCE_DIR=../../src
WARNINGS= #-Wall #-Wextra -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
	-Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused

CFLAGS=$(WARNINGS) -mcpu=cortex-m3 -mthumb -DLPC1768 -I../../src/lpc17xx_lib/include -I../../src/uKernel -I../../src -I../../src/cmsis_boot -I../../src/cmsis -g -O0

LINKER_FLAGS=-mcpu=cortex-m3 -mthumb -g -nostartfiles -Wl,-Map=$(BIN_DIR)/uKernel.map -O0  -Wl,--gc-sections -L../uKernel -Wl,-T$(LDSCRIPT) -g -o $(BIN_DIR)/uKernel.elf -lc

RTOS_SRC = \
$(RTOS_SOURCE_DIR)/main.c \
$(RTOS_SOURCE_DIR)/heap.c \
$(RTOS_SOURCE_DIR)/hooks.c \
$(RTOS_SOURCE_DIR)/kernel.c \
$(RTOS_SOURCE_DIR)/message_queue.c \
$(RTOS_SOURCE_DIR)/port.c \
$(RTOS_SOURCE_DIR)/sem.c \
$(RTOS_SOURCE_DIR)/stats.c \
$(RTOS_SOURCE_DIR)/task.c \
$(RTOS_SOURCE_DIR)/time.c \
$(RTOS_SOURCE_DIR)/wait_queue.c 

RTOS_INC = $(RTOS_SOURCE_DIR)/heap.h \
$(RTOS_SOURCE_DIR)/hooks.h \
$(RTOS_SOURCE_DIR)/kern_conf.h \
$(RTOS_SOURCE_DIR)/kernel.h \
$(RTOS_SOURCE_DIR)/list.h \
$(RTOS_SOURCE_DIR)/message_queue.h \
$(RTOS_SOURCE_DIR)/port.h \
$(RTOS_SOURCE_DIR)/port_types.h \
$(RTOS_SOURCE_DIR)/sem.h \
$(RTOS_SOURCE_DIR)/stats.h \
$(RTOS_SOURCE_DIR)/task.h \
$(RTOS_SOURCE_DIR)/task_types.h \
$(RTOS_SOURCE_DIR)/time.h \
$(RTOS_SOURCE_DIR)/types.h \
$(RTOS_SOURCE_DIR)/wait_queue.h

ARM_SRC = \
$(ARM_SOURCE_DIR)/lpc17xx_timer.c \
$(ARM_SOURCE_DIR)/lpc17xx_clkpwr.c \
$(ARM_SOURCE_DIR)/lpc17xx_libcfg_default.c \
$(ARM_SOURCE_DIR)/lpc17xx_pinsel.c \
$(ARM_SOURCE_DIR)/lpc17xx_gpio.c \
$(ARM_SOURCE_DIR)/lpc17xx_uart.c \
$(ARM_SOURCE_DIR)/lpc17xx_spi.c \

ARM_INC = \
$(ARM_SOURCE_DIR)/../include/lpc_types.h \
$(ARM_SOURCE_DIR)/../include/lpc17xx_timer.h \
$(ARM_SOURCE_DIR)/../include/lpc17xx_pinsel.h \
$(ARM_SOURCE_DIR)/../include/lpc17xx_clkpwr.h \
$(ARM_SOURCE_DIR)/../include/lpc17xx_libcfg_default.h \
$(ARM_SOURCE_DIR)/../include/lpc17xx_uart.h \
$(ARM_SOURCE_DIR)/../include/lpc17xx_spi.h \




CORTEX_SRC = \
$(CORTEX_SOURCE_DIR)/cmsis_boot/startup/cr_startup_lpc17.c \
$(CORTEX_SOURCE_DIR)/cmsis_boot/system_LPC17xx.c \
$(CORTEX_SOURCE_DIR)/cmsis/core_cm3.c

CORTEX_INC = \
$(CORTEX_SOURCE_DIR)/cmsis/core_cm3.h \
$(CORTEX_SOURCE_DIR)/cmsis_boot/LPC17xx.h \
$(CORTEX_SOURCE_DIR)/cmsis_boot/system_LPC17xx.h 



ARM_OBJ := $(patsubst %.c,%.o,$(notdir $(ARM_SRC)))
ARM_OBJ := $(addprefix $(OBJ_DIR)/,$(ARM_OBJ))

RTOS_OBJ := $(patsubst %.c,%.o,$(notdir $(RTOS_SRC)))
RTOS_OBJ := $(addprefix $(OBJ_DIR)/,$(RTOS_OBJ))


CORTEX_OBJ := $(patsubst %.c,%.o,$(notdir $(CORTEX_SRC)))
CORTEX_OBJ := $(addprefix $(OBJ_DIR)/,$(CORTEX_OBJ))


uKernel.hex : uKernel.elf
	$(OBJCOPY) $(BIN_DIR)/uKernel.elf -O ihex $(BIN_DIR)/uKernel.hex

uKernel.elf : $(CORTEX_OBJ) $(ARM_OBJ) $(RTOS_OBJ)
	$(CC) $(CFLAGS)  $(CORTEX_OBJ) $(ARM_OBJ) $(RTOS_OBJ) -nostartfiles $(LINKER_FLAGS)

$(RTOS_OBJ) : $(OBJ_DIR)/%.o : $(RTOS_SOURCE_DIR)/%.c $(RTOS_INC) Makefile #(INCLUDE DIRS), linker script etc
	$(CC) -c $(CFLAGS) $< -o $@ 

$(ARM_OBJ) : $(OBJ_DIR)/%.o : $(ARM_SOURCE_DIR)/%.c $(ARM_INC) Makefile
	$(CC) -c $(CFLAGS) $< -o $@

$(CORTEX_OBJ) : $(OBJ_DIR)/%.o : $(CORTEX_SRC) $(CORTEX_INC) Makefile
	$(eval LOCAL_VAR = $*.c)
	$(CC) -c $(CFLAGS) $(filter %$(LOCAL_VAR), $(CORTEX_SRC)) -o $@


clean :
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*.elf $(BIN_DIR)/*.hex $(BIN_DIR)/*.map


