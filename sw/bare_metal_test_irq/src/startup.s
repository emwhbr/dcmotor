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
 * Bare Metal Test Irq (bmti) startup code.
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */	

/*****************************************************************/
	.text
	.code 32
	
/*****************************************************************/
	
	.global _reset
	.func	_reset

_reset:
 LDR sp, = __stack_end__	/* set the stack pointer     */
 BL c_main			/* jump to C-entry (main)    */
 B .				/* stay here if main returns */
