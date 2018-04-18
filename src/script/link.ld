ENTRY(boot_entry)
SECTIONS {
	PROVIDE( kernel_start = 0x100000 );
	. = 0x100000;
	.init.text : {
		*(.init.text)
		. = ALIGN(4096);
	}
	.init.data : {
		*(.init.data)
		. = ALIGN(4096);
	}
	.text : AT(ADDR(.text) ) {
        *(.text)
        . = ALIGN(4096);
    }
    .data : AT(ADDR(.data) ) {
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }
    .bss : AT(ADDR(.bss) ) {
        *(.bss)
        . = ALIGN(4096);
    }
    .stab : AT(ADDR(.stab)) {
        *(.stab)
        . = ALIGN(4096);
    }
    .stabstr : AT(ADDR(.stabstr)) {
        *(.stabstr)
        . = ALIGN(4096);
    }
	PROVIDE( kernel_end = . );
	/DISCARD/ : { *(comment) *(eh_frame) }
}
