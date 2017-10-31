export

DEVICE_FAMILY = STM32F4xx
DEVICE_TYPE = STM32F401xE
STARTUP_FILE = stm32f401xe
SYSTEM_FILE = stm32f4xx

CMSIS = Drivers/CMSIS
CMSIS_DEVSUP = $(CMSIS)/Device/ST/$(DEVICE_FAMILY)/
CMSIS_OPT = -D$(DEVICE_TYPE) -DUSE_HAL_DRIVER
OTHER_OPT = "-D__weak=__attribute__((weak))" "-D__packed=__attribute__((__packed__))" 
CPU = -mthumb -mcpu=cortex-m4 
SYSTEM = arm-none-eabi


#LDSCRIPT = STM32F407ZG_FLASH.ld
LDSCRIPT = "./STM32F401RETx_FLASH.ld"

SRCDIR := Src/
INCDIR := Inc/

RTOS_DIR = uKernel

LIBDIR := Drivers/

#LIBINC := $(shell find $(INCDIR) -name *.h -printf "-I%h/\n"|sort|uniq)
#LIBINC += $(shell find $(LIBDIR) -name *.h -printf "-I%h/\n"|sort|uniq)
#LIBINC += $(shell find $(MDLDIR) -name *.h -printf "-I%h/\n"|sort|uniq|sed -e 's/lwip\///'|sed -e 's/arch\///')

LIBINC := -IInc
LIBINC += -IDrivers/CMSIS/Include
LIBINC += -IDrivers/CMSIS/Device/ST/STM32F4xx/Include
LIBINC += -IDrivers/STM32F4xx_HAL_Driver/Inc/
LIBINC += -IDrivers/BSP/STM32F4xx-Nucleo/

RTOS := ./$(RTOS_DIR)/uKernel.o

LIBS := ./$(LIBDIR)/STM32F4xx_HAL_Driver/libstm32fw.a

LIBS += -lm
CC      = $(SYSTEM)-gcc
CCDEP   = $(SYSTEM)-gcc
LD      = $(SYSTEM)-gcc
AR      = $(SYSTEM)-ar
AS      = $(SYSTEM)-gcc
OBJCOPY = $(SYSTEM)-objcopy
OBJDUMP	= $(SYSTEM)-objdump
GDB		= $(SYSTEM)-gdb
SIZE	= $(SYSTEM)-size
OCD	= sudo ~/openocd-git/openocd/src/openocd \
		-s ~/openocd-git/openocd/tcl/ \
		-f interface/stlink-v2.cfg \
		-f target/stm32f4x_stlink.cfg


  
# INCLUDES = -I$(SRCDIR) $(LIBINC)
INCLUDES = $(LIBINC)
WARNINGS= -Wall -Wextra -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
	-Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused

CFLAGS  = $(WARNINGS) $(CPU) $(CMSIS_OPT) $(OTHER_OPT) -Wall -fno-common -fno-strict-aliasing -O2 $(INCLUDES) -g -Wfatal-errors -g 
ASFLAGS = $(CFLAGS) -x assembler-with-cpp
LDFLAGS = -Wl,--gc-sections,-Map=$*.map,-cref -T $(LDSCRIPT) $(CPU)
ARFLAGS = cr
OBJCOPYFLAGS = -Obinary
OBJDUMPFLAGS = -S

STARTUP_OBJ = $(CMSIS_DEVSUP)/Source/Templates/gcc/startup_$(STARTUP_FILE).o
SYSTEM_OBJ = $(CMSIS_DEVSUP)/Source/Templates/system_$(SYSTEM_FILE).o

BIN = main.bin

OBJS = $(sort \
 $(patsubst %.c,%.o,$(wildcard Src/*.c)) \
 $(patsubst %.s,%.o,$(wildcard Src/*.s)) \
 $(STARTUP_OBJ) \
 $(SYSTEM_OBJ))

all: $(BIN)

reset:
	$(OCD) -c init -c "reset run" -c shutdown
	#$(GDB) main.out <reset.gdb

flash: $(BIN)
	$(OCD) -c init -c "reset halt" \
	               -c "flash write_image erase "$(BIN)" 0x08000000" \
			       -c "reset run" \
	               -c shutdown

$(BIN): main.out
	$(OBJCOPY) $(OBJCOPYFLAGS) main.out $(BIN)
	$(OBJDUMP) $(OBJDUMPFLAGS) main.out > main.list
	$(SIZE) main.out
	@echo Make finished

main.out: $(LIBS) $(OBJS) $(RTOS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(RTOS) $(LIBS)


$(LIBS): libs
$(RTOS): rtos

rtos:
	@$(MAKE) all -C $(RTOS_DIR)

libs:
	@$(MAKE) -C $(LIBDIR)

libclean: clean
	@$(MAKE) -C $(LIBDIR) clean

clean:
	-rm -f $(OBJS)
	-rm -f main.list main.out main.hex main.map main.bin .depend
	@$(MAKE) -C $(RTOS_DIR) clean

depend dep: .depend

include .depend

.depend: Src/*.c
	$(CCDEP) $(CFLAGS) -MM $^ | sed -e 's@.*.o:@Src/&@' > .depend 

.c.o:
	@echo cc $<
	@$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	@echo as $<
	@$(AS) $(ASFLAGS) -c -o $@ $<
