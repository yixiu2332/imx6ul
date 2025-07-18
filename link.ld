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

    . = ALIGN(4);
    __bss_start = .;
    .bss ALIGN(4) : {
        *(.bss)
        *(COMMON)
    }
    __bss_end = .;

}