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

#include "pwm.h"
#include "sam9l9260.h"

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static volatile AT91S_PMC *g_pmc  = (AT91S_PMC *) AT91C_BASE_PMC;
static volatile AT91S_PIO *g_piob = (AT91S_PIO *) AT91C_BASE_PIOB;
static volatile AT91S_TC  *g_tc3  = (AT91S_TC *) AT91C_BASE_TC3;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void pwm_initialize(void)
{
  g_pmc->PMC_PCER = 1 << AT91C_ID_TC3;  /* enable peripheral clock for timer/counter 3 */

  g_piob->PIO_PDR = AT91C_PB0_TIOA3; /* enable peripheral control of TIOA3 (PB0)*/
  g_piob->PIO_BSR = AT91C_PB0_TIOA3; /* peripheral B selection*/

  g_tc3->TC_CCR = AT91C_TC_CLKDIS; /* disable counter clock */

  g_tc3->TC_CMR = AT91C_TC_ASWTRG_SET      |      /* software trigger sets TIOA      */
                  AT91C_TC_ACPC_TOGGLE     |      /* RC compare toggle TIOA          */
                  AT91C_TC_ACPA_TOGGLE     |      /* RA compare toggle TIOA          */
                  AT91C_TC_WAVE            |      /* waveform mode                   */
                  AT91C_TC_WAVESEL_UP_AUTO |      /* automatic trigger on RC compare */
                  AT91C_TC_CLKS_TIMER_DIV1_CLOCK; /* TIMER_CLOCK1 = MCK/2            */
  
  /* Assumes MCK=100MHz, clock selected = MCK/2=50MHz (T=20ns) */
  g_tc3->TC_RA = 0x0000; /* 0 % duty */
  g_tc3->TC_RC = 0xffff; /* 65535 (T=20ns x 65535 = 1.3107ms, F=763Hz PWM frequency) */

  g_tc3->TC_CCR = AT91C_TC_CLKEN; /* enable counter clock          */
  //g_tc3->TC_CCR = AT91C_TC_SWTRG; /* reset and start counter clock */
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

void pwm_set_duty(uint16_t duty_ctrl)
{
  /* don't allow duty control value 0x0000 or 0xffff,
   * this will generate 50% duty */
  if (duty_ctrl == 0) {
    duty_ctrl = 0x0001;
  }
  if (duty_ctrl == 0xffff) {
    duty_ctrl = 0xfffe;
  }

  /* apply new duty control */
  g_tc3->TC_RA = duty_ctrl;
  g_tc3->TC_CCR = AT91C_TC_SWTRG; /* reset and start counter clock */
}
