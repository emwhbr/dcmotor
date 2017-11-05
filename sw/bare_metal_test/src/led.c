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

#include "led.h"
#include "sam9l9260.h"

/****************************************************************************
 *               Global variables
 ****************************************************************************/
volatile AT91S_PIO *g_pioa = (AT91S_PIO *) AT91C_BASE_PIOA;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void led_initialize(void)
{
  /* set the PIO controller in PIO mode */
  g_pioa->PIO_PER = (SAM9L9260_STAT_LED| SAM9L9260_PWR_LED);
  
  /* configure the pins to output */ 
  g_pioa->PIO_OER = (SAM9L9260_STAT_LED | SAM9L9260_PWR_LED);
  
  /* disable pull-up resitors */
  g_pioa->PIO_PPUDR = (SAM9L9260_STAT_LED | SAM9L9260_PWR_LED);
}

/*****************************************************************/

void led_on(unsigned int led)
{
  if (led == SAM9L9260_STAT_LED) {
    g_pioa->PIO_CODR = SAM9L9260_STAT_LED; /* inverted, 0 for ON */
  } else {
    g_pioa->PIO_SODR = SAM9L9260_PWR_LED;
  }
}

/*****************************************************************/

void led_off(unsigned int led)
{
  if (led == SAM9L9260_STAT_LED) {
    g_pioa->PIO_SODR = SAM9L9260_STAT_LED; /* inverted, 1 for OFF */
  } else {
    g_pioa->PIO_CODR = SAM9L9260_PWR_LED;
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
