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
 * Bare metal test (bmt) startup code.
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */	

.global _Reset
_Reset:
 LDR sp, =stack_top
 BL c_entry
 B .
