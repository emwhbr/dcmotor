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
 * Bare Metal Test (bmt) linker script.
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */	

ENTRY(_Reset)
SECTIONS
{
 . = 0x22200000;
 .startup . : { ./obj/startup.o(.text) }
 .text : { *(.text) }
 .data : { *(.data) }
 .bss : { *(.bss) }
 . = . + 0x1000; /* 4kB of stack memory */
 stack_top = .;
}
