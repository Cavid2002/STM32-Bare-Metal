ENTRY(_reset)

MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 10K
}

SECTIONS {
    .vector : {
        KEEP(*(.vector*))
    } > FLASH

    .text : {
        *(.text*)
    } > FLASH

    .rodata : {
        *(.rodata*)
    } > FLASH

    .data : {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > SRAM AT> FLASH

    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON*)
        _ebss = .;
    } > SRAM

    _estack = ORIGIN(SRAM) + LENGTH(SRAM);
    _ldata = LOADADDR(.data);
}