CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
ARCH=arm-none-eabi-ar
LDSCRIPT=..\..\arm-gcc-link.ld 
OBJ_DIR = obj
BIN_DIR = bin
RTOS_SOURCE_DIR=..\..\src\uKernel
ARM_SOURCE_DIR=..\..\lpc17xx_lib\source
CORTEX_SOURCE_DIR=..\..\src
WARNINGS= #-Wall #-Wextra -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
	-Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused

CFLAGS=$(WARNINGS) -mcpu=cortex-m3 -mthumb -DLPC1768 -I..\..\lpc17xx_lib\include -I..\..\src\uKernel -I..\..\src -I..\..\src\cmsis_boot -I..\..\src\cmsis -g

LINKER_FLAGS=-mcpu=cortex-m3 -mthumb -g -nostartfiles -Wl,-Map=$(BIN_DIR)/uKernel.map -O0  -Wl,--gc-sections -L..\uKernel -Wl,-T..\..\arm-gcc-link.ld -g -o $(BIN_DIR)/uKernel.elf
RTOS_SRC = \
$(RTOS_SOURCE_DIR)\main.c \
$(RTOS_SOURCE_DIR)\heap.c \
$(RTOS_SOURCE_DIR)\hooks.c \
$(RTOS_SOURCE_DIR)\kernel.c \
$(RTOS_SOURCE_DIR)\message_queue.c \
$(RTOS_SOURCE_DIR)\port.c \
$(RTOS_SOURCE_DIR)\sem.c \
$(RTOS_SOURCE_DIR)\stats.c \
$(RTOS_SOURCE_DIR)\task.c \
$(RTOS_SOURCE_DIR)\time.c \
$(RTOS_SOURCE_DIR)\wait_queue.c 
#$(RTOS_SOURCE_DIR)\..\dotmatgl.c

ARM_SRC = \
$(ARM_SOURCE_DIR)\lpc17xx_timer.c \
$(CORTEX_SOURCE_DIR)\cmsis_boot\startup\cr_startup_lpc17.c \
$(ARM_SOURCE_DIR)\lpc17xx_clkpwr.c \
$(CORTEX_SOURCE_DIR)\cmsis_boot\system_LPC17xx.c \
$(CORTEX_SOURCE_DIR)\cmsis\core_cm3.c \
$(ARM_SOURCE_DIR)\lpc17xx_libcfg_default.c 
# $(ARM_SOURCE_DIR)\lpc17xx_gpio.c \
# $(ARM_SOURCE_DIR)\lpc17xx_pwm.c \
# $(ARM_SOURCE_DIR)\lpc17xx_adc.c \
$(ARM_SOURCE_DIR)\lpc17xx_pinsel.c \



ARM_ODIR = obj
RTOS_ODIR = obj
CORTEX_ODIR = obj

# ARM_OBJ = $( patsubst %.c, obj/%.o, $(subst /,-,$(ARM_SRC)))
# RTOS_OBJ = $( patsubst %.c, obj/%.o, $(subst /,-,$(RTOS_SRC)))
RTOS_OBJ = $(RTOS_SRC:.c=.o)
ARM_OBJ = $(ARM_SRC:.c=.o)

uKernel.hex : uKernel.elf
	$(OBJCOPY) $(BIN_DIR)/uKernel.elf -O ihex $(BIN_DIR)/uKernel.hex

uKernel.elf : $(RTOS_OBJ) $(ARM_OBJ) 
	$(CC) $(CFLAGS) $(RTOS_OBJ) $(ARM_OBJ) -nostartfiles $(LINKER_FLAGS)

$(RTOS_OBJ) : %.o : %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

$(ARM_OBJ) : %.o : %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@
# $(RTOS_OBJ) : $(RTOS_SRC)
# 	$(CC) -c $(CFLAGS)  $< -o $@

# $(ARM_OBJ) : $(ARM_SRC) 
# 	$(CC) -c $(CFLAGS)  $< -o $@

clean :
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*.elf $(BIN_DIR)/*.hex $(BIN_DIR)/*.map


# $(ARM_ODIR)/%.o : $(ARM_SRC)
# 	$(CC) -c $(CFLAGS)  $< -o $@

# $(CORTEX_ODIR)/%.o : $(CORTEX_SOURCE_DIR)/cmsis/%.c $(CORTEX_SOURCE_DIR)/cmsis_boot/%.c $(CORTEX_SOURCE_DIR)/cmsis_boot/satrtup/%.c
# 	$(CC) -c $(CFLAGS)  $< -o $@

# $(RTOS_ODIR)/%.o : $(RTOS_SRC)
# 	$(CC) -c $(CFLAGS)  $< -o $@








###################################################################################################
# CC=arm-none-eabi-gcc
# OBJCOPY=arm-none-eabi-objcopy
# ARCH=arm-none-eabi-ar
# OBJ_DIR = obj
# BIN_DIR = bin

# #CRT0=boot.s
# WARNINGS=-Wall -Wextra -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
# 		-Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused

# #
# # CFLAGS common to both the THUMB and ARM mode builds
# #
# CFLAGS=$(WARNINGS) -mcpu=cortex-m3 -mthumb -D LPC1768 -I..\..\lpc17xx_lib\include -I..\..\src\uKernel -I..\..\src -I..\..\src\cmsis_boot -I..\..\src\cmsis

# # ifeq ($(USE_THUMB_MODE),YES)
# # 	CFLAGS += -mthumb-interwork -D THUMB_INTERWORK
# # 	THUMB_FLAGS=-mthumb
# # endif


# LINKER_FLAGS=-mcpu=cortex-m3 -mthumb -g -nostartfiles -Wl,-Map=$(BIN_DIR)/uKernel.map -O0  -Wl,--gc-sections -L..\uKernel -Wl,-T..\..\arm-gcc-link.ld -g -o $(BIN_DIR)/uKernel.elf #-lm #-lgcc -lc

# RTOS_SOURCE_DIR=..\..\src\uKernel
# ARM_SOURCE_DIR=..\..\lpc17xx_lib\source
# CORTEX_SOURCE_DIR=..\..\src

# #
# # Source files that can be built to THUMB mode.
# #
# RTOS_SRC = \
# $(RTOS_SOURCE_DIR)\main.c \
# $(RTOS_SOURCE_DIR)\heap.c \
# $(RTOS_SOURCE_DIR)\hooks.c \
# $(RTOS_SOURCE_DIR)\kernel.c \
# $(RTOS_SOURCE_DIR)\message_queue.c \
# $(RTOS_SOURCE_DIR)\port.c \
# $(RTOS_SOURCE_DIR)\sem.c \
# $(RTOS_SOURCE_DIR)\stats.c \
# $(RTOS_SOURCE_DIR)\task.c \
# $(RTOS_SOURCE_DIR)\time.c \
# $(RTOS_SOURCE_DIR)\wait_queue.c \
# $(RTOS_SOURCE_DIR)\..\dotmatgl.c

# ARM_SRC = \
# $(ARM_SOURCE_DIR)\lpc17xx_clkpwr.c \
# $(ARM_SOURCE_DIR)\lpc17xx_libcfg_default.c \
# $(ARM_SOURCE_DIR)\lpc17xx_gpio.c \
# $(ARM_SOURCE_DIR)\lpc17xx_pinsel.c \
# $(ARM_SOURCE_DIR)\lpc17xx_pwm.c \
# $(ARM_SOURCE_DIR)\lpc17xx_adc.c \
# $(ARM_SOURCE_DIR)\lpc17xx_timer.c \
# $(CORTEX_SOURCE_DIR)\cmsis\core_cm3.c \
# $(CORTEX_SOURCE_DIR)\cmsis_boot\system_LPC17xx.c \
# $(CORTEX_SOURCE_DIR)\cmsis_boot\startup\cr_startup_lpc17.c \
# $(CORTEX_SOURCE_DIR)\syscalls\syscalls.c


# #
# # Source files that must be built to ARM mode.
# #
# #ARM_SRC = \
# #$(RTOS_SOURCE_DIR)/portable/GCC/ARM7_LPC2000/portISR.c \
# #serial/serialISR.c

# #
# # Define all object files.
# #
# # RTOS_OBJ = $( patsubst %.c, %.o, $(RTOS_SRC))
# # ARM_OBJ = $( patsubst %.c, %.o, $(ARM_SRC))
# # RTOS_OBJ = $(RTOS_SRC:.c=.o)
# # ARM_OBJ = $(ARM_SRC:.c=.o)
# # ARM_OBJ = $( patsubst %.c, obj/%.o, $(ARM_SRC))
# # RTOS_OBJ = $( patsubst %.c, obj/%.o, $(RTOS_SRC))
 


# uKernel.hex : uKernel.elf
# 	$(OBJCOPY) $(BIN_DIR)/uKernel.elf -O ihex $(BIN_DIR)/uKernel.hex

# uKernel.elf : $(RTOS_OBJ) $(ARM_OBJ) 
# 	$(CC) $(CFLAGS) $(RTOS_SRC) $(ARM_SRC) -nostartfiles $(LINKER_FLAGS)

# $(RTOS_OBJ) : $(OBJ_DIR)/%.o : $(RTOS_SRC) Makefile #/%.c Makefile
# 	$(CC) -c $(CFLAGS) $< -o $@

# $(ARM_OBJ) : $(OBJ_DIR)/%.o : $(ARM_SRC) Makefile #/%.c Makefile
# 	$(CC) -c $(CFLAGS) $< -o $@

# clean :
# 	rm -rf $(OBJ_DIR)\*.o $(BIN_DIR)\*.elf
# 	touch Makefile









	


