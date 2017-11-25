/* ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 * File Name           : dataflash.h
 * Object              : ATMEL DataFlash Header File
 * Creation            : NLe Jul 12th 2006
 *---------------------------------------------------------------------------
 */
#ifndef _SPI_H
#define _SPI_H

/* Chip Select 0 : NPCS0 %1110 */
#define AT91C_SPI_PCS0_DATAFLASH	0xE
/* Chip Select 1 : NPCS1 %1101 */
#define AT91C_SPI_PCS1_DATAFLASH	0xD
/* Chip Select 2 : NPCS2 %1011 */
#define AT91C_SPI_PCS2_DATAFLASH	0xB
/* Chip Select 3 : NPCS3 %0111 */
#define AT91C_SPI_PCS3_DATAFLASH	0x7

#define	_SPI_MODE_0	(AT91C_SPI_NCPHA)
#define	_SPI_MODE_3	(AT91C_SPI_CPOL)

#if	defined(SPI_MODE_0)
#define	SPI_MODE	_SPI_MODE_0
#elif	defined(SPI_MODE_3)
#define	SPI_MODE	_SPI_MODE_3
#else
#define	SPI_MODE	_SPI_MODE_0
#endif

#if	defined(SPI_BOOT_CS0)
#define	AT91C_SPI_PCS_DATAFLASH		AT91C_SPI_PCS0_DATAFLASH
#elif	defined(SPI_BOOT_CS1)
#define	AT91C_SPI_PCS_DATAFLASH		AT91C_SPI_PCS1_DATAFLASH
#elif	defined(SPI_BOOT_CS2)
#define	AT91C_SPI_PCS_DATAFLASH		AT91C_SPI_PCS2_DATAFLASH
#elif	defined(SPI_BOOT_CS3)
#define	AT91C_SPI_PCS_DATAFLASH		AT91C_SPI_PCS3_DATAFLASH
#else
#define	AT91C_SPI_PCS_DATAFLASH		AT91C_SPI_PCS0_DATAFLASH
#endif


/* SPI CLOCK */
#if	defined(SPI_CLK)
#define	AT91C_SPI_CLK		(SPI_CLK)
#else
#define AT91C_SPI_CLK 		 5000000
#endif

#endif
