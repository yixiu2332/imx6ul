KERNELDIR := $(CURDIR)/linux-6.12.40
obj-m := chrdevbase.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules 

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules 