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

#include "motor.h"
#include "encoder.h"
#include "pwm.h"

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void motor_initialize()
{
  /*
   * initalize motor DIRECTION control
   */
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB; /* enable peripheral clock for PIOB */

  /* direction pin (PB1) */
  AT91C_BASE_PIOB->PIO_PER = MOTOR_PIN_DIR;   /* enable pio */
  AT91C_BASE_PIOB->PIO_OER = MOTOR_PIN_DIR;   /* output enable */
  AT91C_BASE_PIOB->PIO_PPUDR = MOTOR_PIN_DIR; /* disable pull-up resistor */

  /*
   * initialize motor ENCODER and PWM control
   */
  encoder_initialize();
  pwm_initialize();

  /* set initial duty to zero, clockwise */
  motor_ctrl(true, 0x0000);
}

/*****************************************************************/

void motor_ctrl(bool clockwise,
		uint16_t duty)
{
  /* set motor direction */
  if (clockwise) {
    AT91C_BASE_PIOB->PIO_CODR = MOTOR_PIN_DIR;
  }
  else {
    AT91C_BASE_PIOB->PIO_SODR = MOTOR_PIN_DIR;
  }

  /* set motor speed */
  pwm_set_duty(duty);
}

/*****************************************************************/

void motor_zero_shaft_position(void)
{
  encoder_set_counter(0);
}

/*****************************************************************/

uint16_t motor_get_shaft_position(void)
{
  return encoder_get_gearbox_shaft_position();
}

/*****************************************************************/

uint16_t motor_shaft_max_position(void)
{
  return ( ENCODER_CPR * (uint16_t)ENCODER_GEAR );
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/
