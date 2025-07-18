/* 1. 指定程序入口 */
ENTRY(_start)


SECTIONS {
    . = 0x80000000;

    .text ALIGN(4) : {
        *(.text)
    }

    .rodata ALIGN(4) : { *(.rodata*) }
    .data ALIGN(4) : ALIGN(4) {
        *(.data)
    }

    . = ALIGN(4);/*这行很重要，不然可能会出现__bss_start地址没有对齐，然后清空bss段时程序崩溃*/
    __bss_start = .;
    .bss ALIGN(4) : {
        *(.bss)
        *(COMMON)
    }
    __bss_end = .;

}