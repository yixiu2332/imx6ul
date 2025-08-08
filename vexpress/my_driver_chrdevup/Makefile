ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-
# 模块名称 (这是你希望生成的 .ko 文件的名字)
obj-m := chrdevup.o

KERNELDIR := /home/xiu/code/imx6ul/vexpress/linux-6.12.40

CURRENT_PATH := $(shell pwd)

# 默认的构建目标
all: kernel_modules

# 编译内核模块的目标
kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
# 清理编译生成的文件
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean