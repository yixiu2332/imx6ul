CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET ?= main

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

LIBPATH := -lgcc -L /usr/local/arm/gcc-linaro/lib/gcc/arm-linux-gnueabihf/4.9.4

INCDIRS := 	devices \
			bsp/clk \
			bsp/led \
			bsp/delay \
			bsp/beep \
			bsp/gpio \
			bsp/key \
			bsp/int \
			bsp/exti \
			bsp/epittimer \
			bsp/uart \
			stdio/include \


SRCDIRS := 	project \
			bsp/clk \
			bsp/led \
			bsp/delay \
			bsp/beep \
			bsp/gpio \
			bsp/key \
			bsp/int \
			bsp/exti \
			bsp/epittimer \
			bsp/uart \
			stdio/lib \

INCLUDE := $(patsubst %, -I %, $(INCDIRS))
SFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))

SOBJS := $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS := $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS := $(SOBJS) $(COBJS)

VPATH := $(SRCDIRS)

.PHONY: clean all

all:$(TARGET).bin

obj/:
	@mkdir -p $@

$(TARGET).bin : $(OBJS)
	$(LD) -Tlink.ld -o $(TARGET).elf $^ $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S | obj/
	$(CC) -Wall -nostdlib -fno-builtin -c -g $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c | obj/
	$(CC) -Wall -nostdlib -fno-builtin -Wa,-mimplicit-it=thumb -c -g $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)
