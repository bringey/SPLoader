ENTRY(early)

loadAddr = 0x10000;

SECTIONS
{

    .loader16 0x0 : AT(loadAddr) {
        *(.loader16)
        . = ALIGN(0x200);
    }


    .text (loadAddr + SIZEOF(.loader16)) : AT(loadAddr + SIZEOF(.loader16)) {

        *(.text)
        *(.rodata)

        _etext = .;
    }


    .data : {
        _data = .;
        *(.data)
        _edata = .;
    }

    .bss : {
        _bss = .;
        *(.bss);
        _ebss = .;
    }

}