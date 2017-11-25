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
 * File Name           : at91sam9260ek.h
 * Object              :
 * Creation            : NLe Jul 13th 2006
 *-----------------------------------------------------------------------------
 */
#ifndef _AT91SAM9260EK_H
#define _AT91SAM9260EK_H
/* ******************************************************************* */
/* PMC Settings                                                        */
/*                                                                     */
/* The main oscillator is enabled as soon as possible in the c_startup */
/* and MCK is switched on the main oscillator.                         */
/* PLL initialization is done later in the hw_init() function          */
/* ******************************************************************* */
#define MASTER_CLOCK		(198656000/2)

#define PLL_LOCK_TIMEOUT	1000000

#define PLLA_SETTINGS	0x2060BF09
#define PLLB_SETTINGS	0x10483F0E

/* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
#define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_2)

/* Flash settings - Use a default, which hopefully works for all AT91 chips */
#include	"smc0_setup.h"


/* ******************************************************************* */
/* BootStrap Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#define MACH_TYPE       	0x44B		/* AT91SAM9260-EK */

#define IMG_ADDRESS 		0x8000		/* Image Address in Flash */

#if	defined(IMG_SIZE)
#warning			"IMG_SIZE redefined"
#else
#define	IMG_SIZE		0x38000		/* Image Size in Flash    */
#endif

#if	defined(JUMP_ADDR)
#warning			"JUMP_ADDR redefined"
#else
#define JUMP_ADDR		0x23F00000	/* Final Jump Address 	      */
#endif

/* ******************************************************************* */
/* Application Settings                                                */
/* ******************************************************************* */
#define CFG_DEBUG
#define CFG_FLASH
#define	CFG_SDRAM
#define	CFG_HW_INIT

#endif	/* _AT91SAM9260EK_H */
