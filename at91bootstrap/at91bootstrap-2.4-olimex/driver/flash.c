/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2007, Atmel Corporation

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
 * File Name           : flash.c
 * Object              : Flash High level functions
 * Creation            : USa Aug 14th 2007
 *---------------------------------------------------------------------------
*/

#include "../include/part.h"
#include "../include/main.h"
#include "../include/flash.h"
#include "../include/smc.h"
#include <stdlib.h>

#ifdef CFG_FLASH

/*------------------------------------------------------------------------------*/
/* \fn    smc0_hw_init								*/
/* \brief This function performs Static Memory Controller HW initialization	*/
/* It should be executed BEFORE the slow clock is changed			*/
/* WARNING: The C_Startup has done VERY LITTLE at this point			*/
/*------------------------------------------------------------------------------*/
void smc0_hw_init(void)
{
	/* The SMC is either clocked by the Slow clock or by MCLK
	 * MCLK is typically 100 MHz
	 */

unsigned int	setup0,pulse0,cycle0,mode0;

	writel(readl(AT91C_BASE_CCFG + CCFG_EBICSA) |DATABUS_PULLUP_ON|MEMORY_VOLTAGE_3_3V , AT91C_BASE_CCFG + CCFG_EBICSA);

	/* Check : SETUP + PULSE <= CYCLE ! */

	setup0	=	
		((NCS_READ_SETUP & 0x3F)	<< 24)	|	// (128 * bit[5]) + bit[4:0]
		((NRD_SETUP	 & 0x3F)	<< 16)	|	// (128 * bit[5]) + bit[4:0]
		((NCS_WR_SETUP	 & 0x3F)	<<  8)	|	// (128 * bit[5]) + bit[4:0]
		((NWE_SETUP	 & 0x3F)	<<  0);		// (128 * bit[5]) + bit[4:0]
	writel(setup0, AT91C_BASE_SMC + SMC_SETUP0);
	
	pulse0	= 
		((NCS_READ_PULSE & 0x3F)	<< 24)	|	// (128 * bit[5]) + bit[4:0]
		((NRD_PULSE	 & 0x3F)	<< 16)	|	// (128 * bit[5]) + bit[4:0]
		((NCS_WR_PULSE	 & 0x3F)	<<  8)	|	// (128 * bit[5]) + bit[4:0]
		((NWE_PULSE	 & 0x3F)	<<  0);		// (128 * bit[5]) + bit[4:0]
	writel(pulse0, AT91C_BASE_SMC + SMC_PULSE0);

	cycle0	= 
		((NRD_CYCLE	 & 0x1FF)	<< 16)	|	// (256 * bit[8:7]) + bit[6:0]
		((NWE_CYCLE	 & 0x1FF)	<<  0);		// (256 * bit[8:7]) + bit[6:0]
	writel(cycle0, AT91C_BASE_SMC + SMC_CYCLE0);

	mode0	= (PS|PMEN|TDF_MODE|TDF_CYCLES|DBW|BAT|EXNW_MODE|WRITE_MODE|READ_MODE);
	writel(mode0, AT91C_BASE_SMC + SMC_CTRL0);
}

/*------------------------------------------------------------------------------*/
/* \fn    flash_is_boot_valid							*/
/* \brief Check that the first bytes of the buffer are valid ARM vectors	*/
/*------------------------------------------------------------------------------*/
static unsigned int flash_is_boot_valid(unsigned char *buffer)
{
	int i = 3;

 	/* Verify if the 28 first bytes of the sram correspond to ARM vectors
	   The sixth ARM vector contain the size of the code */
    	while(i < 28)
    	{
		if (i != 23)
		{
			if ((buffer[i] != 0xEA) && (buffer[i] != 0xE5) )
				return -1;
		}
		i+=4;
    	}

    	return 0;
}

/*------------------------------------------------------------------------------*/
/* \fn    load_flash								*/
/* \brief This function loads flash content to specified address		*/
/*------------------------------------------------------------------------------*/	
int load_flash(unsigned int img_addr, unsigned int img_size, unsigned int img_dest)
{
	unsigned char *src, *dst;
	src = (unsigned char *) img_addr;
	dst = (unsigned char *) img_dest;

	flash_hw_init();

	if (flash_is_boot_valid((unsigned char*)img_addr))
		return -1;

	while (img_size > 0) {
		*dst++ = *src++;
		img_size--;
	}
   	return 0;
}

#endif /* CFG_FLASH */
