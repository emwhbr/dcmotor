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
#include "sam9l9260.h"

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static volatile AT91S_PMC *g_pmc  = (AT91S_PMC *) AT91C_BASE_PMC;
static volatile AT91S_PIO *g_piob = (AT91S_PIO *) AT91C_BASE_PIOB;
static volatile AT91S_TC  *g_tc4  = (AT91S_TC *) AT91C_BASE_TC4;
static volatile AT91S_TCB *g_tcb1 = (AT91S_TCB *) AT91C_BASE_TCB1;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void encoder_initialize(void)
{
  g_pmc->PMC_PCER = 1 << AT91C_ID_TC4;  /* enable peripheral clock for timer/counter 4 */

  g_piob->PIO_PDR = AT91C_PB17_TCLK4; /* enable peripheral control of TCLK4 (PB17)*/
  g_piob->PIO_BSR = AT91C_PB17_TCLK4; /* peripheral B selection*/

  g_tc4->TC_CCR = AT91C_TC_CLKDIS; /* disable counter clock */

  g_tcb1->TCB_BMR = AT91C_TCB_TC1XC1S_TCLK1; /* TCLK4 connected to XC4 */

  g_tc4->TC_CMR = AT91C_TC_CLKS_XC1; /* capture mode, selected clock XC4 */

  g_tc4->TC_CCR = AT91C_TC_CLKEN; /* enable counter clock */
  g_tc4->TC_CCR = AT91C_TC_SWTRG; /* reset and start counter clock */
}

/*****************************************************************/

uint16_t encoder_get_value(void)
{
  return (g_tc4->TC_CV & 0xffff);
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
