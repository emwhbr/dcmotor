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
 * File Name           : sam9_l9260.c
 * Object              :
 * Creation            : NLe Jul 13th 2006
 * ODi Nov 9th         : dstp #3507 "Bad PIO descriptors in at91samxxxek.c"
 * ... May 5th         : dimitar at olimex.com: modified for SAM9-L9260
 *-----------------------------------------------------------------------------
 */
#include "../../include/part.h"
#include "../../include/gpio.h"
#include "../../include/pmc.h"
#include "../../include/debug.h"
#include "../../include/sdramc.h"
#include "../../include/main.h"
#ifdef CFG_NANDFLASH
#include "../../include/nandflash.h"
#endif
#ifdef CFG_DATAFLASH
#include "../../include/dataflash.h"
#endif

static inline unsigned int get_cp15(void)
{
	unsigned int value;
	__asm__("mrc p15, 0, %0, c1, c0, 0" : "=r" (value));
	return value;
}

static inline void set_cp15(unsigned int value)
{
	__asm__("mcr p15, 0, %0, c1, c0, 0" : : "r" (value));
}

/*----------------------------------------------------------------------------*/
/* \fn    hw_init							      */
/* \brief This function performs very low level HW initialization	      */
/* This function is invoked as soon as possible during the c_startup	      */
/* The bss segment must be initialized					      */
/*----------------------------------------------------------------------------*/
void hw_init(void)
{
	unsigned int cp15;
	
	/* Configure PIOs */
	const struct pio_desc hw_pio[NR_HW_PIO_PINS] = {
	  {AT91C_PIN_PB(14), 0, PIO_DEFAULT, PIO_PERIPH_A}, /* RX (DBGU)             */
	  {AT91C_PIN_PB(15), 0, PIO_DEFAULT, PIO_PERIPH_A}, /* TX (DBGU)             */
	  {AT91C_PIN_PA(9),  0, PIO_DEFAULT, PIO_OUTPUT},   /* PWR_LED  (yellow) Off */
	  {AT91C_PIN_PA(6),  0, PIO_DEFAULT, PIO_OUTPUT},   /* STAT_LED (green)  On  */
	  {AT91C_PIN_PC(15), 0, PIO_DEFAULT, PIO_INPUT}     /* BUT                   */
	};

	/* Disable watchdog */
	writel(AT91C_WDTC_WDDIS, AT91C_BASE_WDTC + WDTC_WDMR);

	/* Configure the PIO controller and set initial state of the LEDs */
	pio_setup(hw_pio);

	/* At this stage the main oscillator is supposed to be enabled
	 * PCK = MCK = MOSC */

	/* Configure PLLA = MOSC * (PLL_MULA + 1) / PLL_DIVA */
	pmc_cfg_plla(PLLA_SETTINGS, PLL_LOCK_TIMEOUT);

	/* Switch MCK on PLLA output PCK = PLLA = 2 * MCK */
	pmc_cfg_mck(MCKR_SETTINGS, PLL_LOCK_TIMEOUT);

	/* Configure PLLB */
	pmc_cfg_pllb(PLLB_SETTINGS, PLL_LOCK_TIMEOUT);

	/* Configure CP15 */
	cp15 = get_cp15();
	cp15 |= I_CACHE;
	set_cp15(cp15);

#ifdef CFG_DEBUG
	/* Enable Debug messages on the DBGU */
	dbg_init(BAUDRATE(MASTER_CLOCK, 115200));

	dbg_print("\n\rAT91Bootstrap : ");
	dbg_print(AT91_BOOTSTRAP_VER);
	dbg_print("\n\r");
#endif /* CFG_DEBUG */

	/* Initialize the matrix */
	writel(readl(AT91C_BASE_CCFG + CCFG_EBICSA) | AT91C_EBI_CS1A_SDRAMC, AT91C_BASE_CCFG + CCFG_EBICSA);

	/* Configure SDRAM Controller */
	sdram_init(SDRAMC_CR_SETTINGS,    /* Configuration Register */
		   SDRAMC_TR_SETTINGS);	  /* Refresh Timer Register */

	/* Dummy code, for setting breakpoint with OpenOCD */
	__asm__("nop");
	__asm__("nop");
}

/*------------------------------------------------------------------------------*/
/* \fn    sdramc_hw_init							*/
/* \brief This function performs SDRAMC HW initialization			*/
/*------------------------------------------------------------------------------*/
void sdramc_hw_init(void)
{
	/* Configure PIOs */
/*	const struct pio_desc sdramc_pio[] = {
		{"D16", AT91C_PIN_PC(16), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D17", AT91C_PIN_PC(17), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D18", AT91C_PIN_PC(18), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D19", AT91C_PIN_PC(19), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D20", AT91C_PIN_PC(20), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D21", AT91C_PIN_PC(21), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D22", AT91C_PIN_PC(22), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D23", AT91C_PIN_PC(23), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D24", AT91C_PIN_PC(24), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D25", AT91C_PIN_PC(25), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D26", AT91C_PIN_PC(26), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D27", AT91C_PIN_PC(27), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D28", AT91C_PIN_PC(28), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D29", AT91C_PIN_PC(29), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D30", AT91C_PIN_PC(30), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"D31", AT91C_PIN_PC(31), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{(char *) 0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};
*/
	/* Configure the SDRAMC PIO controller to output PCK0 */
/*	pio_setup(sdramc_pio); */

	writel(0xFFFF0000, AT91C_BASE_PIOC + PIO_ASR(0));
	writel(0xFFFF0000, AT91C_BASE_PIOC + PIO_PDR(0));

}

#ifdef CFG_NANDFLASH

/*------------------------------------------------------------------------------*/
/* \fn    nandflash_hw_init							*/
/* \brief NandFlash HW init							*/
/*------------------------------------------------------------------------------*/
void nandflash_hw_init(void)
{
	/* Configure PIOs */
	const struct pio_desc nand_pio[2] = {
	  {AT91C_PIN_PC(13), 0, PIO_PULLUP, PIO_INPUT},  /* RDY_BSY */
	  {AT91C_PIN_PC(14), 0, PIO_PULLUP, PIO_OUTPUT}  /* NANDCS  */
	};

	/* Setup Smart Media, first enable the address range of CS3 in HMATRIX user interface */
	writel(readl(AT91C_BASE_CCFG + CCFG_EBICSA) | AT91C_EBI_CS3A_SM, AT91C_BASE_CCFG + CCFG_EBICSA);
		    
	/* Configure SMC CS3 */
 	writel((AT91C_SM_NWE_SETUP | AT91C_SM_NCS_WR_SETUP | AT91C_SM_NRD_SETUP | AT91C_SM_NCS_RD_SETUP), AT91C_BASE_SMC + SMC_SETUP3);
  	writel((AT91C_SM_NWE_PULSE | AT91C_SM_NCS_WR_PULSE | AT91C_SM_NRD_PULSE | AT91C_SM_NCS_RD_PULSE), AT91C_BASE_SMC + SMC_PULSE3);
	writel((AT91C_SM_NWE_CYCLE | AT91C_SM_NRD_CYCLE)						, AT91C_BASE_SMC + SMC_CYCLE3);
	writel((AT91C_SMC_READMODE | AT91C_SMC_WRITEMODE | AT91C_SMC_NWAITM_NWAIT_DISABLE | 
  		AT91C_SMC_DBW_WIDTH_EIGTH_BITS | AT91C_SM_TDF)						, AT91C_BASE_SMC + SMC_CTRL3);

	/* Configure the PIO controller */
	writel((1 << AT91C_ID_PIOC), PMC_PCER + AT91C_BASE_PMC);
	pio_setup(nand_pio);
}

/*------------------------------------------------------------------------------*/
/* \fn    nandflash_cfg_16bits_dbw_init						*/
/* \brief Configure SMC in 16 bits mode						*/
/*------------------------------------------------------------------------------*/
// EMWHBR: NAND data bus width is 8 bits, removed to shrink size of the final bin-file
//
//void nandflash_cfg_16bits_dbw_init(void)
//{
//	writel(readl(AT91C_BASE_SMC + SMC_CTRL3) | AT91C_SMC_DBW_WIDTH_SIXTEEN_BITS, AT91C_BASE_SMC + SMC_CTRL3);
//}

#endif /* #ifdef CFG_NANDFLASH */
