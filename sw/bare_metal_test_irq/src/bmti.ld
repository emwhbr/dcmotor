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
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */	

ENTRY(_reset)

SDRAM_APP_START_ADDR = 0x22200000; /* load image here  */
C_STACK_SIZE         = 0x1000;     /* 4KB stack memory */
C_HEAP_SIZE          = 0;          /* no heap          */

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
 
    .bss : { *(.bss) }

    .heap : {
        __heap_start__ = . ;
        . = . + C_HEAP_SIZE;
        . = ALIGN(4);
        __heap_end__ = . ;
    }

    .stack : {
        __stack_start__ = . ;
        . = . + C_STACK_SIZE;
        . = ALIGN(4);
        __stack_end__ = . ;
    }
}
