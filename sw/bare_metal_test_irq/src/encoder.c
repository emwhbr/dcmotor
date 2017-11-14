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

#include "encoder.h"
#include "arm_exc.h"
#include "sam9l9260.h"

#include "console.h" // TBD: playing with encoder interrupt

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static volatile int g_encoder_cnt = 0;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void encoder_initialize(void)
{
  //AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOC; /* enable peripheral clock for PIOC */
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_IRQ1;

  AT91C_BASE_PIOC->PIO_PDR = AT91C_PC15_IRQ1; /* enable peripheral control of IRQ1 (PC15)*/
  AT91C_BASE_PIOC->PIO_BSR = AT91C_PC15_IRQ1; /* peripheral B selection*/

  //AT91C_BASE_PIOC->PIO_PER = AT91C_PC15_IRQ1;   /* enable pio */
  //AT91C_BASE_PIOC->PIO_ODR = AT91C_PC15_IRQ1;   /* output disable */
  //AT91C_BASE_PIOC->PIO_PPUDR = AT91C_PC15_IRQ1; /* disable pull-up resistor */
  //AT91C_BASE_PIOC->PIO_PPUER = AT91C_PC15_IRQ1;   /* enable pull-up resistor */
}

/*****************************************************************/

int encoder_get_counter(void)
{
  uint32_t encoder_cnt;
  ARM_INT_KEY_TYPE int_lock_key;  

  ARM_INT_LOCK(int_lock_key);
  encoder_cnt = g_encoder_cnt;
  ARM_INT_UNLOCK(int_lock_key);

  return encoder_cnt;
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

void encoder_isr_output_a(void)
{
  /* 
   * Application ISR: Encoder output A 
   */

  ARM_INT_KEY_TYPE int_lock_key;

  /* clear this interrupt */
  /* done automatically when reading AIC_IVR (bsp_isr_irq) */
  
  console_putln("ENCA");

  ARM_INT_LOCK(int_lock_key);
  g_encoder_cnt++;
  ARM_INT_UNLOCK(int_lock_key);
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
