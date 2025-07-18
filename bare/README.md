## 裸机程序使用说明

1. 请自行安装QEMU
2. 使用make编译项目，得到elf或者bin文件。
3. 执行以下命令运行启动程序：
    `qemu-system-arm \
    -M mcimx6ul-evk \
    -cpu cortex-a7 \
    -nographic \
    -kernel ${KERNEL_FILE} \
    -m 512M \
    -d unimp,guest_errors \`
4. 如果需要调试加上`-s -S `参数