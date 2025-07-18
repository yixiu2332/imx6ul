## 裸机程序使用说明

我是跟着正点原子视频学的，代码大部分也是参考例程的。

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
- bsp 是板上外设的驱动，虽然很多外设该模型没有实现。
- devices 是NXP官方给出的库文件，简化寄存器开发。
- obj 是编译过程中产生的中间文件。
- project 是main.c文件和一些重要内容。

#### 注意事项
1. 模型选择mcimx6ul-evk，据说这就是为imx6ul开发板专门适配的。
2. 很多硬件外设没有被实现，所以就不要测试了。
3. 测试程序主要使用串口，推荐第一步就开启；不然只能通过GDB一步步运行，然后写指令查看运行结果，很苦逼。该模型已经默认实现了串口1到终端控制器的重映射，只要初始化串口后就可以使用了。

