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

#include "button.h"
#include "arm_exc.h"

#include "console.h" // TBD: playing with button interrupt

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static volatile bool g_was_pressed = false;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void button_initialize(void)
{
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOC; /* enable peripheral clock for PIOC */
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_IRQ1; /* enable peripheral clock for IRQ1 */

  AT91C_BASE_PIOC->PIO_PDR = BUTTON_PIN_USER_BUTTON; /* enable peripheral control of IRQ1 (PC15)*/
  AT91C_BASE_PIOC->PIO_BSR = BUTTON_PIN_USER_BUTTON; /* peripheral B selection*/
}

/*****************************************************************/

bool button_is_pressed(void)
{
  /* button has pull-up resistor, GND when pressed */
  return ( !(AT91C_BASE_PIOC->PIO_PDSR & BUTTON_PIN_USER_BUTTON) );
}

/*****************************************************************/

bool button_was_pressed(void)
{
  bool val;
  ARM_INT_KEY_TYPE int_lock_key;  

  ARM_INT_LOCK(int_lock_key);
  val = g_was_pressed;
  g_was_pressed = false;
  ARM_INT_UNLOCK(int_lock_key);

  return val;
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

void button_isr_pressed(void)
{
  /* 
   * Application ISR: User button pressed
   */

  ARM_INT_KEY_TYPE int_lock_key;

  /* clear this interrupt */
  /* done automatically when reading AIC_IVR (bsp_isr_irq) */
  
  console_putln("BUTT");

  ARM_INT_LOCK(int_lock_key);
  g_was_pressed = true;
  ARM_INT_UNLOCK(int_lock_key);
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
