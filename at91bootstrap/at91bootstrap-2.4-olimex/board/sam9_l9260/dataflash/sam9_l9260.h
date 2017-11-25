/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
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
 * File Name           : sam9_l9260.h
 * Object              :
 * Creation            : NLe Jul 13th 2006
 *-----------------------------------------------------------------------------
 */
#ifndef _SAM9_L9260_H
#define _SAM9_L9260_H
#include	"spi.h"
/* ******************************************************************* */
/* PMC Settings                                                        */
/*                                                                     */
/* The main oscillator is enabled as soon as possible in the c_startup */
/* and MCK is switched on the main oscillator.                         */
/* PLL initialization is done later in the hw_init() function          */
/* ******************************************************************* */
#if 0
  #define MASTER_CLOCK		(200704000/4)
  #define PLL_LOCK_TIMEOUT	1000000

  #define PLLA_SETTINGS		0x20613F09
  #define PLLB_SETTINGS		0x10483F0E

  /* Switch MCK on PLLA output PCK = PLLA = 4 * MCK */
  #define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK \
		  	| AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_3)
#elif 1
  #define MASTER_CLOCK		(180224000/2)
  #define PLL_LOCK_TIMEOUT	1000000

  #define PLLA_SETTINGS		0x20573F09
  #define PLLB_SETTINGS		0x10483F0E

  /* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
  #define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK \
		  	| AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_2)
#elif 0
  #define MASTER_CLOCK		(159744000/2)
  #define PLL_LOCK_TIMEOUT	1000000

  #define PLLA_SETTINGS		0x204D3F09
  #define PLLB_SETTINGS		0x10483F0E

  /* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
  #define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK \
		  	| AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_2)
#elif 0
  #define MASTER_CLOCK		(147456000/2)
  #define PLL_LOCK_TIMEOUT	1000000

  #define PLLA_SETTINGS		0x20473F09
  #define PLLB_SETTINGS		0x10483F0E

  /* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
  #define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK \
		  	| AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_2)
#elif 0 /* not supported, for test purposes only */
  #define MASTER_CLOCK		(198656000/2)
  #define PLL_LOCK_TIMEOUT	1000000

  #define PLLA_SETTINGS	0x2060BF09
  #define PLLB_SETTINGS	0x10483F0E

  /* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
  #define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_2)
#endif


/* ******************************************************************* */
/* DataFlash Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#define AT91C_BASE_SPI	AT91C_BASE_SPI0
#define AT91C_ID_SPI	AT91C_ID_SPI0

/* SPI CLOCK */
#define AT91C_SPI_CLK 		 5000000
/* AC characteristics */
/* DLYBS = tCSS= 250ns min and DLYBCT = tCSH = 250ns */
#define DATAFLASH_TCSS		(0x1a << 16)	/* 250ns min (tCSS) <=> 12/48000000 = 250ns */
#define DATAFLASH_TCHS		(0x1 << 24)	/* 250ns min (tCSH) <=> (64*1+SCBR)/(2*48000000) */

#define DF_CS_SETTINGS 		((SPI_MODE) | (AT91C_SPI_DLYBS & DATAFLASH_TCSS) | (AT91C_SPI_DLYBCT & DATAFLASH_TCHS) | ((MASTER_CLOCK / AT91C_SPI_CLK) << 8))


/* ******************************************************************* */
/* BootStrap Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#define MACH_TYPE       	1501		/* SAM9-L9260 */

#define IMG_ADDRESS 		0x8400		/* Image Address in DataFlash */

#if	defined(IMG_SIZE)
#warning			"IMG_SIZE redefined"
#else
#define	IMG_SIZE		0x32000		/* Image Size in DataFlash    */
#endif

#if	defined(JUMP_ADDR)
#warning			"JUMP_ADDR redefined"
#else
#define JUMP_ADDR		0x23F00000	/* Final Jump Address 	      */
#endif

/* ******************************************************************* */
/* Application Settings                                                */
/* ******************************************************************* */
//#define CFG_DEBUG
#define CFG_DATAFLASH
#define	CFG_SDRAM
#define	CFG_HW_INIT


#endif	/* _SAM9_L9260_H */
