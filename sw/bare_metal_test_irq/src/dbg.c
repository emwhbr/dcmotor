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

#include "dbg.h"

/****************************************************************************
 *               Global variables
 ****************************************************************************/
/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void dbg_initialize(void)
{
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB; /* enable peripheral clock for PIOB */

  /* debug output pin 1 (PB1) */
  AT91C_BASE_PIOB->PIO_PER = DBG_PIN_1;   /* enable pio */
  AT91C_BASE_PIOB->PIO_OER = DBG_PIN_1;   /* output enable */
  AT91C_BASE_PIOB->PIO_PPUDR = DBG_PIN_1; /* disable pull-up resistor */

  /* debug output pin 2 (PB2) */
  AT91C_BASE_PIOB->PIO_PER = DBG_PIN_2;   /* enable pio */
  AT91C_BASE_PIOB->PIO_OER = DBG_PIN_2;   /* output enable */
  AT91C_BASE_PIOB->PIO_PPUDR = DBG_PIN_2; /* disable pull-up resistor */
}

/*****************************************************************/

void dbg_pin_on(uint32_t pin)
{
  AT91C_BASE_PIOB->PIO_SODR = pin;
}

/*****************************************************************/

void dbg_pin_off(uint32_t pin)
{
  AT91C_BASE_PIOB->PIO_CODR = pin;
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
