/************************************************************************
 *                                                                      *
 * Copyright (C) 2017 Bonden i Nol (hakanbrolin@hotmail.com)            *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 ************************************************************************/

/*
 * Bare Metal Test Irq (bmti) linker script.
 *  - https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu
 *
 *  - Building Bare-Metal ARM Systems with GNU
 *    http://www.state-machine.com
 */	

ENTRY(_reset)

/* load image here, SDRAM  */
SDRAM_APP_START_ADDR = 0x22200000;

/* stack memory sizes */
C_STACK_SIZE   = 0x1000; /* 4KB stack */
IRQ_STACK_SIZE = 0x1000; /* 4KB stack */
FIQ_STACK_SIZE = 0x1000; /* 4KB stack */
SVC_STACK_SIZE = 0x1000; /* 4KB stack */
ABT_STACK_SIZE = 0x1000; /* 4KB stack */
UND_STACK_SIZE = 0x1000; /* 4KB stack */

/* heap memory size */
C_HEAP_SIZE = 0; /* no heap */

SECTIONS
{
    . = SDRAM_APP_START_ADDR;
 
    /* start-up code */
    .startup : {
        ./obj/startup.o(.text)
    }

    /* interrupt related code */
    .fastcode : {
        *(.text.fastcode)
    }
 
    /* other code */
    .text : { *(.text) }
 
    .data : { *(.data) }

    .bss : {
        __bss_start__ = . ;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    }

    /* heap */
    .heap : {
        __heap_start__ = . ;
        . = . + C_HEAP_SIZE;
        . = ALIGN(4);
        __heap_end__ = . ;
    }

    /* stack */
    .stack : {
        __stack_start__ = . ;

	. += IRQ_STACK_SIZE;
        . = ALIGN (4);
        __irq_stack_top__ = . ;

        . += FIQ_STACK_SIZE;
        . = ALIGN (4);
        __fiq_stack_top__ = . ;

        . += SVC_STACK_SIZE;
        . = ALIGN (4);
        __svc_stack_top__ = . ;

        . += ABT_STACK_SIZE;
        . = ALIGN (4);
        __abt_stack_top__ = . ;

        . += UND_STACK_SIZE;
        . = ALIGN (4);
        __und_stack_top__ = . ;

        . += C_STACK_SIZE;
        . = ALIGN (4);
        __c_stack_top__ = . ;

        __stack_end__ = . ;
    }
}
