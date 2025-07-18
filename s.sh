#!/bin/bash
KERNEL_FILE="main.elf"

if [ $# -lt 1 ];then echo "Parameters required";fi #检查参数

case $1 in 
    1)
    echo "开始执行程序"
    qemu-system-arm \
    -M mcimx6ul-evk \
    -cpu cortex-a7 \
    -nographic \
    -kernel ${KERNEL_FILE} \
    -m 512M \
    -d unimp,guest_errors \
    ;;
    2)
    echo "调试中"
    qemu-system-arm \
    -M mcimx6ul-evk \
    -cpu cortex-a7 \
    -nographic \
    -kernel ${KERNEL_FILE} \
    -m 512M \
    -d unimp,guest_errors \
    -s -S
    ;;
    0)
    gdb-multiarch  ${KERNEL_FILE} -ex "target remote localhost:1234" -ex "b main"
    ;; 
esac
