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

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static volatile int g_encoder_cnt = 0;

/* 
 * Reading quadrature encoders with interrupts:
 *  - http://makeatronics.blogspot.se/2013/02/efficiently-reading-quadrature-with.html
 */

static const int8_t g_encoder_table[] = 
  {
        /*  state                    */
        /* old  new                  */
        /* ab   ab   hex   direction */
        /* ------------------------- */
    0,  /* 00   00   0x0   none      */
   -1,  /* 00   01   0x1   -1        */
    1,  /* 00   10   0x2   +1        */
    0,  /* 00   11   0x3   none      */
    1,  /* 01   00   0x4   +1        */
    0,  /* 01   01   0x5   none      */
    0,  /* 01   10   0x6   none      */
   -1,  /* 01   11   0x7   -1        */
   -1,  /* 10   00   0x8   -1        */
    0,  /* 10   01   0x9   none      */
    0,  /* 10   10   0xa   none      */
    1,  /* 10   11   0xb   +1        */
    0,  /* 11   00   0xc   none      */
    1,  /* 11   01   0xd   +1        */
   -1,  /* 11   10   0xe   -1        */
    0   /* 11   11   0xf   none      */
  };

static uint8_t g_encoder_state = 0b1111; /* initial encoder state */

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

void encoder_get_gearbox_shaft(const int encoder_counter,
			       bool *clockwise,
			       int *rev,
			       uint16_t *pos)
{
  float gearbox_rev;
  int gearbox_n_rev;
  float gearbox_part_rev;

  /* calculate gearbox output shaft position */
  gearbox_rev = ((float) encoder_counter) / (ENCODER_CPR * ENCODER_GEAR);
  gearbox_n_rev = (int) gearbox_rev;
  gearbox_part_rev = (gearbox_rev - gearbox_n_rev) * (ENCODER_CPR * ENCODER_GEAR);

  /* return revolutions (rev) and part of
   * revolution (pos) expressed as position 0 - (ENCODER_CPR * ENCODER_GEAR)
   * clockwise indicates direction */
  if (gearbox_part_rev >= 0.0f) {
    *clockwise = true;
  }
  else {
    *clockwise = false;
    gearbox_n_rev *= -1.0;
    gearbox_part_rev *= -1.0f;
  }
  *rev = gearbox_n_rev;
  *pos = (uint16_t) gearbox_part_rev;
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

void encoder_isr_output_ab(void)
{
  /* 
   * Application ISR: Encoder outputs A & B 
   */

  ARM_INT_KEY_TYPE int_lock_key;

  uint8_t encoder_a;
  uint8_t encoder_b;
  uint8_t encoder_state_new;

  /* clear this interrupt */
  uint32_t volatile piob_isr = AT91C_BASE_PIOB->PIO_ISR;

  /* get new encoder state */
  encoder_a = ( (AT91C_BASE_PIOB->PIO_PDSR & ENCODER_PIN_ENCA) ? 1 : 0 );
  encoder_b = ( (AT91C_BASE_PIOB->PIO_PDSR & ENCODER_PIN_ENCB) ? 1 : 0 );
  encoder_state_new = (encoder_a << 1) | encoder_b;

  /* save old encoder state */
  g_encoder_state = (g_encoder_state << 2) & 0x0f;

  /* encoder state = old | new */
  g_encoder_state |= encoder_state_new;

  /* lookup table gives us direction and count */
  ARM_INT_LOCK(int_lock_key);
  g_encoder_cnt += g_encoder_table[g_encoder_state];
  ARM_INT_UNLOCK(int_lock_key);
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
