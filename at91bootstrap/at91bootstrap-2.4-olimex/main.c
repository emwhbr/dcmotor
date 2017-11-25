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
 * File Name           : main.c
 * Object              :
 * Creation            : ODi Apr 19th 2006
 *-----------------------------------------------------------------------------
 */
#include "include/part.h"
#include "include/main.h"
#include "include/debug.h"
#include "include/nandflash.h"
#include "include/gpio.h"

/*------------------------------------------------------------------------------*/
/* Function Name       : main							*/
/* Object              : Main function						*/
/* Input Parameters    : none							*/
/* Output Parameters   : True							*/
/*------------------------------------------------------------------------------*/
int main(void)
{
  unsigned char boot_mode;
  unsigned int img_addr, img_size, img_dest;
  volatile unsigned int *p_kernel_params = (unsigned int *) KERNEL_PARAM_START_ADDR;

/* ================== 1st step: Hardware Initialization ================= */

	/* Performs the hardware initialization */
	hw_init();

/* ==================== 2nd step: Load from media ==================== */

	/* Check if BUT is pressed */
	if ( pio_get_value(AT91C_PIN_PC(15)) ) {
	  /* Jump directly to kernel and bypass U-Boot */
	  boot_mode = BOOT_MODE_KERNEL;
	  img_addr  = IMG_ADDR_KERNEL;
	  img_size  = IMG_SIZE_KERNEL;
	  img_dest  = JUMP_ADDR_KERNEL - UBOOT_HEADER_SIZE;
	}
	else {
	  /* U-Boot requested (BUT is pressed) */
	  boot_mode = BOOT_MODE_UBOOT;
	  img_addr  = IMG_ADDR_UBOOT;
	  img_size  = IMG_SIZE_UBOOT;
	  img_dest  = JUMP_ADDR_UBOOT;
	}
	
	/* Load from Nandflash to SDRAM */
	if ( load_nandflash(img_addr, img_size, img_dest) ) {
	    goto load_failed;
	}

/* ==================== 3th step: Start the application =================== */	

	/* Jump to the Image Address */
	if (boot_mode == BOOT_MODE_UBOOT) {
	  dbg_print("U-Boot@[");
	  dbg_print_hex(img_dest);
	  dbg_print("]\r\n");

	  /* Start U-Boot */
	  return img_dest;
	}
	else {
	  dbg_print("Kernel@[");
	  dbg_print_hex(img_dest + UBOOT_HEADER_SIZE);
	  dbg_print("]\r\n");

	  /* Make sure any previous kernel parameter list,
	   * passed by U-Boot is destroyed. This will force
	   * kernel to use its built-in parameters. */

	  /* Create an empty tagged list */
	  *p_kernel_params++ = 0x2;
	  *p_kernel_params++ = 0x54410001; /* ATAG_CORE */
	  *p_kernel_params++ = 0x0;
	  *p_kernel_params   = 0x0;        /* ATAG_NONE */
	  
	  /* Prepare jump to kernel startup entry point 
	   * See kernel source arch/arm/kernel/head.S */
	  asm volatile("mov    r0, #0");           /* r0 = 0             */
	  asm volatile("ldr    r1, =1501");        /* r1 = machine nr    */
	  asm volatile("ldr    r2, =0x20000000");  /* r2 = atags pointer */

	  /* Start kernel, assumes parameters are compiled into kernel image */
	  asm volatile("ldr    r3, =0x20008000");
	  asm volatile("bx     r3");

	  /* Start kernel, assumes parameters are compiled into kernel image */
	  //return (img_dest + UBOOT_HEADER_SIZE);
	}

load_failed:
	pio_set_value(AT91C_PIN_PA(9), 1); /* PWR_LED (yellow) On */
	dbg_print("\r\nLoad error\r\n");
	while(1) {
	  ;
	}
}
