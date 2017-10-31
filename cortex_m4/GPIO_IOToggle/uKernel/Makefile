export

RTOS_OUT = uKernel.o

RTOS_INCS := $(shell find . -name "*.[h]")
RTOS_SRCS := $(shell find . -name "*.[cs]")
RTOS_OBJS = $(sort $(patsubst %.c,%.o,$(RTOS_SRCS)))


.PHONY: all
all: $(RTOS_OUT)

$(RTOS_OUT): $(RTOS_OBJS)  # In order to link the RTOS_OBJS together run gcc with -r
                           # e.g. gcc -Wl,-r obj1.o obj2.o obj3.o -o obj.o -nostdlib
	$(CC) -Wl,-r $(RTOS_OBJS) -o uKernel.o -nostdlib

# Make would implicitly compile the RTOS_OBJS but we do it here explicitly
$(RTOS_OBJS) : %.o : %.c $(RTOS_INCS) Makefile
	$(CC) -c $(CFLAGS) -I../Drivers/CMSIS/Include $< -o $@


.PHONY: clean
clean:
	-rm -f $(RTOS_OBJS) $(RTOS_OUT)

