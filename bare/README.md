## 裸机程序使用说明

#### 准备工作
1. 自行安装QEMU
2. Makefile需要静态库文件，如LIBPATH变量，静态库压缩包已在lib文件夹中给出。
3. 使用make编译项目，得到elf或者bin文件。
4. 执行以下命令运行启动程序：
    `qemu-system-arm \
    -M mcimx6ul-evk \
    -cpu cortex-a7 \
    -nographic \
    -kernel ${KERNEL_FILE} \
    -m 512M \
    -d unimp,guest_errors \`
5. 如果需要调试加上`-s -S `参数,然后使用GDB远程调试`gdb-multiarch  ${KERNEL_FILE} -ex "target remote localhost:1234" -ex "b main"`

#### 项目结构说明
- bsp 是板上外设的驱动，虽然很多外设该模型没有实现但也保留了驱动。
- devices 是NXP官方给出的库文件，简化寄存器开发。。
- project 是main.c文件和其他重要文件。

#### 注意事项
1. 模型选择mcimx6ul-evk，据说这就是为imx6ul开发板专门适配的。
2. 很多硬件外设没有被实现，比如蜂鸣器，LED、外部中断等就没有测试。还有IO的复用功能也没有实现，复用配置寄存器也不存在，如果访问的话会报告`iomuxc: unimplemented device write `。
3. 测试程序主要使用串口，推荐第一步就开启；不然只能通过GDB一步步运行，然后写指令查看运行结果，很苦逼。该模型已经默认实现了串口1到终端控制器的重映射，只要初始化串口后就可以使用了。
4. 由于qemu的定时器都是模拟出来的，所以计时肯定比实际的硬件慢；不过这点无伤大雅，延时时间写短一点就好了。


#### 踩的坑
1. qemu的CBAR寄存器有问题，按照ARM架构规范它默认存储的是GIC的基地址（0x00A00000），但实际上是0x0；而且官方的库函数中都是用的CBAR寄存器提供的地址，导致我的GIC中断配置一直不起作用，排查耗时一天，但好在收获了对中断配置更深入的理解。


#### mcimx6ul-evk模型的外设具体实现
###### 已经实现的设备
- CPU (Cortex-A7)
- GIC (作为 A7MPCORE 的一部分)
- 时钟、复位、电源管理 (CCM, SRC, GPCv2)
- 安全和实时时钟 (SNVS)
- 通用 I/O (GPIOs)
- 多种定时器 (GPTs, EPITs)
- 多种串行通信接口 (ECSPIs, I2Cs, UARTs)
- 网络接口 (Ethernets)
- USB 接口和 PHY (USBs, USB PHYs)
- SD/MMC 接口 (USDHCs)
- 看门狗 (Watchdogs)
- ROM 和 OCRAM 内存

###### 未实现的设备
- A7MPCORE DAP (Debug Access Port)
- MMDC (Multi-Mode DDR Controller) 配置寄存器
- OCOTP (One-Time Programmable)
- QSPI (Quad Serial Peripheral Interface)
- CAAM (Cryptographic Acceleration and Assurance Module)
- USBMISC (USB Miscellaneous)
- IOMUXC (I/O Multiplexer Controller)
- IOMUXC GPR (General Purpose Register)
- SDMA (Smart DMA)
- SAIs (Synchronous Audio Interface)
- PWMs (Pulse Width Modulation)
- Audio ASRC (Asynchronous Sample Rate Converter)
- CANs (Controller Area Network)
- APBH_DMA (Advanced Peripheral Bus Host DMA)
- ADCs (Analog-to-Digital Converters)
- LCDIF (LCD Interface)
- CS (Central Security Unit)
- TZASC (TrustZone Address Space Controller)
