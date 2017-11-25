OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(reset)
SECTIONS
{
        . = ALIGN(4);
        .text : 
        { 
        	_stext = .;
        	*(.text)
		_etext = .;
        }

        . = ALIGN(4);
        .rodata : { *(.rodata) }

        . = ALIGN(4);
        .data : { /* collect all initialized .bss sections that go into FLASH  */
        	_sdata = .;
        	*(.data)
        	_edata = .;
        }

        . = ALIGN(4);
        .bss : { /* collect all uninitialized .bss sections that go into FLASH  */
            __bss_start__ = .;
            *(.bss)
            __bss_end__ = .;
        }
}
end = .;  /* define a global symbol marking the end of application RAM */

