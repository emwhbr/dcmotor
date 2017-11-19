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

#include "adc.h"
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

void adc_initialize(void)
{
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_ADC;  /* enable peripheral clock for ADC */

  AT91C_BASE_PIOC->PIO_PDR = ADC_PIN_INPUT; /* enable peripheral control of AD0 (PC0)*/
  AT91C_BASE_PIOC->PIO_ASR = ADC_PIN_INPUT; /* peripheral A selection*/

  /* assumes MCK=100MHz (T=10ns)
   * select ADC frequency = 2.5MHz (max 5MHz according to datasheet)
   * Tadc = 0.4us
   *
   * ADCClock = MCK / ((PRESCAL+1) * 2) ==> PRESCAL=19
   *
   * Startup time = (STARTUP+1) * 8 * Tadc
   * Startup time = 16us ==> STARTUP=4
   *
   * Sample&Hold = (SHTIM+1) * Tadc
   * Sample&Hold = 10xTadc = 4us ==> SHTIM=9
   */
  AT91C_BASE_ADC->ADC_MR =
    (19 <<  8)                  | /* PRESCAL=19             */
    ( 4 << 16)                  | /* STARTUP=4              */
    ( 9 << 24)                  | /* SHTIM=9                */
    AT91C_ADC_LOWRES_10_BIT     | /* 10-bit resolution      */
    AT91C_ADC_SLEEP_NORMAL_MODE | /* normal mode            */
    AT91C_ADC_TRGEN_DIS;          /* conversion start by sw */
  
  AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH0; /* enable ADC channel 0 (AD0) */
}

/*****************************************************************/

uint16_t adc_get_value(void)
{
  /* start conversion */
  AT91C_BASE_ADC->ADC_CR = AT91C_ADC_START;

  dbg_pin_on(DBG_PIN_1); // TBD: playing with ADC

  /* wait for conversion to complete (AD0) */
  while ( !(AT91C_BASE_ADC->ADC_SR & AT91C_ADC_EOC0) ) {;}

  dbg_pin_off(DBG_PIN_1);  // TBD: playing with ADC

  /* read conversion result (AD0) */
  return (uint16_t) (AT91C_BASE_ADC->ADC_CDR0 & 0x3ff);
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

