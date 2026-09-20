ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rw):ORIGIN =0X08000000, LENGTH =512K
    SRAM(rwx):ORIGIN =0x20000000, LENGTH =128K
}

_estack = ORIGIN(SRAM)+LENGTH(SRAM);

__max_heap_size = 0x200;
__max_stack_size = 0x400;

SECTIONS
{
    .text   :
    {
        . = ALIGN(4);
        *(.isr_vector_tbl)
        *(.text)
        *(.rodata)
        . = ALIGN(4);
        _etext = .;
    }>FLASH

    .data   :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    } > SRAM AT> FLASH

    .bss    :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        . = ALIGN(4);
        _ebss = .;
    }>SRAM
}