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
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB; /* enable peripheral clock for PIOB */

  /* encoder output A (PB4) */
  AT91C_BASE_PIOB->PIO_PER = ENCODER_PIN_ENCA;   /* enable pio */
  AT91C_BASE_PIOB->PIO_ODR = ENCODER_PIN_ENCA;   /* output disable */
  AT91C_BASE_PIOB->PIO_PPUDR = ENCODER_PIN_ENCA; /* disable pull-up resistor */

  /* encoder output B (PB5) */
  AT91C_BASE_PIOB->PIO_PER = ENCODER_PIN_ENCB;   /* enable pio */
  AT91C_BASE_PIOB->PIO_ODR = ENCODER_PIN_ENCB;   /* output disable */
  AT91C_BASE_PIOB->PIO_PPUDR = ENCODER_PIN_ENCB; /* disable pull-up resistor */
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

void encoder_isr_output_ab(void)
{
  /* 
   * Application ISR: Encoder outputs A & B 
   */

  ARM_INT_KEY_TYPE int_lock_key;

  /* clear this interrupt */
  uint32_t volatile piob_isr = AT91C_BASE_PIOB->PIO_ISR;
  
  /* check interrupt cause */
  if (piob_isr & ENCODER_PIN_ENCA) {
    console_putln("ENCA");
  }
  if (piob_isr & ENCODER_PIN_ENCB) {
    console_putln("ENCB");
  }

  if (AT91C_BASE_PIOB->PIO_PDSR & ENCODER_PIN_ENCA) {
    console_putln("  A-H");
  }
  else {
    console_putln("  A-L");
  }
  if (AT91C_BASE_PIOB->PIO_PDSR & ENCODER_PIN_ENCB) {
    console_putln("  B-H");
  }
  else {
    console_putln("  B-L");
  }

  ARM_INT_LOCK(int_lock_key);
  g_encoder_cnt++;
  ARM_INT_UNLOCK(int_lock_key);
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
