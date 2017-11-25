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
 * this list of conditions and the disclaimer below.
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
 * Creation            : NLe Sep 28th 2006
 *-----------------------------------------------------------------------------
 */
#ifndef _SAM9_L9260_HBR_H
#define _SAM9_L9260_HBR_H
#include	"spi.h"

/* ******************************************************************* */
/* Fastboot Settings                                                   */
/*                                                                     */
/* ******************************************************************* */
#define CONFIG_FASTBOOT
//#undef CONFIG_FASTBOOT

/* ******************************************************************* */
/* PIO Settings                                                        */
/*                                                                     */
/* ******************************************************************* */
#define NR_HW_PIO_PINS  5  /*  RX       - PB14  (DBGU)
			    *  TX       - PB15  (DBGU)
			    *  PWR_LED  - PA9   (yellow)
			    *  STAT_LED - PA6   (green)
			    *  BUT      - PC15
 			    */

/* ******************************************************************* */
/* PMC Settings                                                        */
/*                                                                     */
/* The main oscillator is enabled as soon as possible in the c_startup */
/* and MCK is switched on the main oscillator.                         */
/* PLL initialization is done later in the hw_init() function          */
/* ******************************************************************* */
#ifndef CONFIG_FASTBOOT

/* Normal settings */
#define MASTER_CLOCK		(200704000/4)   /* 50.176 MHz */
#define PLL_LOCK_TIMEOUT	1000000

#define PLLA_SETTINGS		0x20613F09      /* Matches: 50.176 MHz */
#define PLLB_SETTINGS		0x10483F0E

/* Switch MCK on PLLA output PCK = PLLA = 4 * MCK */
#define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK \
		  	| AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_3)

#else

/* Fastboot settings */
//#define MASTER_CLOCK		(198656000/2)   /* 99.328 MHz */
#define MASTER_CLOCK		(204800000/2)   /* 102.400 MHz */
#define PLL_LOCK_TIMEOUT	1000000

//#define PLLA_SETTINGS		0x2060BF09      /* Matches: 99.328 MHz */
#define PLLA_SETTINGS		0x2063BF09      /* Matches: 102.400 MHz */
#define PLLB_SETTINGS		0x10487F0E

/* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
#define MCKR_SETTINGS	(AT91C_PMC_CSS_PLLA_CLK \
		  	| AT91C_PMC_PRES_CLK | AT91C_PMC_MDIV_2)

#endif

/* ******************************************************************* */
/* SDRAM Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#ifndef CONFIG_FASTBOOT

/* Normal settings */
#define SDRAMC_CR_SETTINGS  0x85227259  /* Configuration Register 
					 * NC = 9-bit
					 * NR = 13-bit
					 * NB = 4 banks
					 * CAS = 2 cycles
					 * DBW = 32-bit
					 * TWR = 2 cycles
					 * TRC = 2 cycles
					 * TRP = 2 cycles
					 * TRCD = 2 cycles
					 * TRAS = 5 cycles
					 * TXSR = 8 cycles */
#else

/* Fastboot settings */
#define SDRAMC_CR_SETTINGS  0x85237259  /* Configuration Register 
					 * NC = 9-bit
					 * NR = 13-bit
					 * NB = 4 banks
					 * CAS = 2 cycles
					 * DBW = 32-bit
					 * TWR = 2 cycles
					 * TRC = 7 cycles
					 * TRP = 3 cycles
					 * TRCD = 2 cycles
					 * TRAS = 5 cycles
					 * TXSR = 8 cycles */
#endif

/* Refresh Timer Register 
 * 64 ms, 8K-rows --> 64000/8192=7.8125 */
#define SDRAMC_TR_SETTINGS  ((MASTER_CLOCK * 7) / 1000000)

/* ******************************************************************* */
/* NandFlash Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#define AT91C_SMARTMEDIA_BASE	0x40000000

#define AT91_SMART_MEDIA_ALE    (1 << 21)	/* our ALE is AD21 */
#define AT91_SMART_MEDIA_CLE    (1 << 22)	/* our CLE is AD22 */

#define NAND_DISABLE_CE() do { *(volatile unsigned int *)AT91C_PIOC_SODR = AT91C_PIO_PC14;} while(0)
#define NAND_ENABLE_CE() do { *(volatile unsigned int *)AT91C_PIOC_CODR = AT91C_PIO_PC14;} while(0)

#define NAND_WAIT_READY() while (!(*(volatile unsigned int *)AT91C_PIOC_PDSR & AT91C_PIO_PC13))


/* ******************************************************************** */
/* SMC Chip Select 3 Timings for NandFlash for MASTER_CLOCK = 100000000.*/
/* Micron 16bits 256Mb for MASTER_CLOCK = 100000000.			*/
/* Please refer to SMC section in AT91SAM9261 datasheet to learn how 	*/
/* to generate these values. 						*/
/* ******************************************************************** */
#define AT91C_SM_NWE_SETUP	(0 << 0)
#define AT91C_SM_NCS_WR_SETUP	(0 << 8)
#define AT91C_SM_NRD_SETUP	(0 << 16)
#define AT91C_SM_NCS_RD_SETUP	(0 << 24)
  
#define AT91C_SM_NWE_PULSE 	(4 << 0)
#define AT91C_SM_NCS_WR_PULSE	(6 << 8)
#ifndef CONFIG_FASTBOOT
/* Normal settings */
#define AT91C_SM_NRD_PULSE	(3 << 16)
#else
/* Fastboot settings */
#define AT91C_SM_NRD_PULSE	(2 << 16)
#endif
#define AT91C_SM_NCS_RD_PULSE	(5 << 24)
  
#define AT91C_SM_NWE_CYCLE 	(6 << 0)
#ifndef CONFIG_FASTBOOT
/* Normal settings */
#define AT91C_SM_NRD_CYCLE	(5 << 16)
#define AT91C_SM_TDF	        (1 << 16)
#else
/* Fastboot settings */
#define AT91C_SM_NRD_CYCLE	(3 << 16)
#define AT91C_SM_TDF	        (0 << 16)
#endif


/* ******************************************************************** */
/* SMC Chip Select 3 Timings for NandFlash for MASTER_CLOCK = 100000000.*/
/* Micron 16bits 256Mb for MASTER_CLOCK = 100000000.			*/
/* Please refer to SMC section in AT91SAM9261 datasheet to learn how 	*/
/* to generate these values. 						*/
/* ******************************************************************** */
/*#define AT91C_SM_NWE_SETUP	(0 << 0)
#define AT91C_SM_NCS_WR_SETUP	(0 << 8)
#define AT91C_SM_NRD_SETUP	(0 << 16)
#define AT91C_SM_NCS_RD_SETUP	(0 << 24)
  
#define AT91C_SM_NWE_PULSE 	(2 << 0)
#define AT91C_SM_NCS_WR_PULSE	(3 << 8)
#define AT91C_SM_NRD_PULSE	(2 << 16)
#define AT91C_SM_NCS_RD_PULSE	(3 << 24)
  
#define AT91C_SM_NWE_CYCLE 	(3 << 0)
#define AT91C_SM_NRD_CYCLE	(3 << 16)

#define AT91C_SM_TDF	        (1 << 16)
*/

/* ******************************************************************* */
/* BootStrap Settings                                                  */
/*                                                                     */
/* ******************************************************************* */
#define MACH_TYPE      		1501		/* SAM9-L9260 */

#define BOOT_MODE_UBOOT   0
#define BOOT_MODE_KERNEL  1
#define BOOT_MODE_BM      2  /* bare-metal (BM) */

#define IMG_ADDR_UBOOT    0x1CF00000	/* U-Boot Address in NandFlash */
#define IMG_ADDR_KERNEL   0x1D300000    /* Kernel Address in NandFlash */
#define IMG_ADDR_BM       0x00020000    /* BM Address in NandFlash */

#ifndef CONFIG_FASTBOOT
#define IMG_SIZE_UBOOT    0x0E0000      /* Normal: U-Boot Size in NandFlash (896KB) */
#define IMG_SIZE_KERNEL   0x300000      /* Normal: Kernel Size in NandFlash (  3MB) */
#else
#define IMG_SIZE_UBOOT    0x040000      /* Fastboot: U-Boot Size in NandFlash (256KB) */
#define IMG_SIZE_KERNEL   0x219800      /* Fastboot: Kernel Size in NandFlash (2.2MB) 
					 * Must be multiple of 2K */
#endif
#define IMG_SIZE_BM       0x020000      /* Normal or Fastboot: BM Size in NandFlash (128KB)*/

#define JUMP_ADDR_UBOOT   0x23F00000	/* Final Jump Address SDRAM (U-Boot) */
#define JUMP_ADDR_KERNEL  0x20008000	/* Final Jump Address SDRAM (Kernel) */
#define JUMP_ADDR_BM      0x22200000	/* Final Jump Address SDRAM (BM)     */

#define UBOOT_HEADER_SIZE  0x40         /* Compensate for U-Boot header:
					 * 0x20008000-40 = 0x20007FC0,
					 * when loading kernel */

/* Passing kernel parameters (ARM):
 * The recommended placement is in the first 16KiB of RAM,
 * usually the start of physical RAM plus 0x100.
*/
#define KERNEL_PARAM_START_ADDR  0x20000000

/* ******************************************************************* */
/* Application Settings                                                */
/* ******************************************************************* */
#undef CFG_DATAFLASH

#define CFG_DEBUG
#define CFG_NANDFLASH
#undef	NANDFLASH_SMALL_BLOCKS	/* NANDFLASH_LARGE_BLOCKS used instead */
#define CFG_SDRAM
#define CFG_HW_INIT

#ifndef CONFIG_FASTBOOT
#define AT91_BOOTSTRAP_VER "HBR-V8NF-Normal  "
#else
#define AT91_BOOTSTRAP_VER "HBR-V8NF-Fast-BM1"
#endif

#endif	/* _SAM9_L9260_HBR_H */
