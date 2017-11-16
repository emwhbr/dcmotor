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

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void led_initialize(void)
{
  /* set the PIOA controller in PIO mode */
  AT91C_BASE_PIOA->PIO_PER = (LED_PIN_STAT | LED_PIN_PWR);
  
  /* configure the pins to output */ 
  AT91C_BASE_PIOA->PIO_OER = (LED_PIN_STAT | LED_PIN_PWR);
  
  /* disable pull-up resitors */
  AT91C_BASE_PIOA->PIO_PPUDR = (LED_PIN_STAT | LED_PIN_PWR);
}

/*****************************************************************/

void led_on(uint32_t pin_led)
{
  if (pin_led == LED_PIN_STAT) {
    AT91C_BASE_PIOA->PIO_CODR = LED_PIN_STAT; /* inverted, 0 for ON */
  } else {
    AT91C_BASE_PIOA->PIO_SODR = LED_PIN_PWR;
  }
}

/*****************************************************************/

void led_off(uint32_t pin_led)
{
  if (pin_led == LED_PIN_STAT) {
    AT91C_BASE_PIOA->PIO_SODR = LED_PIN_STAT; /* inverted, 1 for OFF */
  } else {
    AT91C_BASE_PIOA->PIO_CODR = LED_PIN_PWR;
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
