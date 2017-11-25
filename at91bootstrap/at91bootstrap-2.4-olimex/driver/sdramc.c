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
 * File Name           : sdramc.c
 * Object              :
 * Creation            : NLe Jul 11th 2006
 *-----------------------------------------------------------------------------
 */
#include "../include/sdramc.h"
#include "../include/part.h"
#include "../include/main.h"

#ifdef CFG_SDRAM

//*----------------------------------------------------------------------------
//* \fn    sdram_init
//* \brief Initialize the SDRAM Controller
//*----------------------------------------------------------------------------
int sdram_init(unsigned int sdramc_cr, unsigned int sdramc_tr)
{
  /* Register usage:
   * r0: Argument 1, sdramc_cr (Procedure Call Standard for the ARM Architecture)
   * r1: Argument 2, sdramc_tr (Procedure Call Standard for the ARM Architecture)
   * r2: Base address SDRAMC
   * r3: Base address SDRAM
   * r4: Temporary values 
   * r5: Temporary values */

  /* Preserve registers, according to standard */
  asm volatile("push   {r4,r5}");

  /* Performs the hardware initialization */
  sdramc_hw_init();

  /* The initialization sequence of SDRAMC according to
     AT91SAM9260 Reference Manual, Document: 6221H-ATARM-12-Aug-08.
     See section 22.4.1, SDRAM Device Initialization (Step 1-11). */

  asm volatile("ldr    r2, =0xffffea00\n\t"
	       "ldr    r3, =0x20000000");

  /* Step 1: SDRAM features */
  asm volatile("str    r0, [r2, #0x08]");    /* SDRAMC_CR */

  /* Step 2: Not applicable (mobile SDRAM) */

  /* Step 3: SDRAM memory type */
  asm volatile("mov    r4, #0x0\n\t"
	       "str    r4, [r2, #0x24]");    /* SDRAMC_MDR */

  /* Step 4: A minimum pause of 200 μs */
  asm volatile("ldr    r4, =30000\n\t"
	       "delay_200us:\n\t"
	       "subs   r4, r4, #1\n\t"
	       "bne    delay_200us");

  /* Step 5: NOP command to SDRAM devices */
  asm volatile("mov    r4, #1\n\t"
	       "str    r4, [r2, #0x00]\n\t"  /* SDRAMC_MR      */
	       "str    r4, [r3]");           /* Write to SDRAM */

  /* Step 6: All banks precharge command */
  asm volatile("mov    r4, #2\n\t"
	       "str    r4, [r2, #0x00]\n\t"  /* SDRAMC_MR      */
	       "str    r4, [r3]");           /* Write to SDRAM */

  /* Step 7: 8 x auto-refresh (CBR) cycles */
  asm volatile("mov    r4, #4\n\t"
	       "mov    r5, #8\n\t"
	       "eight_cbr:\n\t"
	       "str    r4, [r2, #0x00]\n\t"  /* SDRAMC_MR      */
	       "str    r4, [r3]\n\t"         /* Write to SDRAM */
	       "subs   r5, r5, #1\n\t"
	       "bne    eight_cbr");

  /* Step 8: Load mode register command */
  asm volatile("mov    r4, #3\n\t"
	       "str    r4, [r2, #0x00]\n\t"  /* SDRAMC_MR      */
	       "str    r4, [r3]");           /* Write to SDRAM */

  /* Step 9: Not applicable (mobile SDRAM) */

  /* Step 10: Normal mode */
  asm volatile("mov    r4, #0\n\t"
	       "str    r4, [r2, #0x00]\n\t"  /* SDRAMC_MR      */
	       "str    r4, [r3]");           /* Write to SDRAM */

  /* Step 11: Refresh rate */
  asm volatile("str    r1, [r2, #0x04]");    /* SDRAMC_TR      */

  /* Restore registers, according to standard */
  asm volatile("pop  {r4,r5}");

  return 0;
}

#endif /* CFG_SDRAM */
