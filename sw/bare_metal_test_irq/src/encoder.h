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

/*
 * DC gearmotor, Pololu-3240
 *  - https://www.pololu.com/product/3240
 *
 * Quadrature encoders explained
 *  - http://www.cui.com/blog/what-is-encoder-ppr-cpr-and-lpr
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <stdbool.h>

#include "sam9l9260.h"

/* quadrature encoder definitions, Pololu-3240 */
#define ENCODER_CPR  (48) /* counts per revolution
			   * number of quadrature decoded states that
			   * exist between the two outputs A and B
			   */

#define ENCODER_PPR  (12) /* pulses per revolution on A or B output
			   * CPR / 4
			   */

#define ENCODER_GEAR  (34.014f) /* motor shaft         : 34.014 revolutions
				 * gearbox output shaft: 1 revolution
				 * motor runs faster than gearbox output
				 * one revolution (gearbox output):
				 *   1 x 34.014 x 48 = 1632,672 encoder counts
				 */

/* encoder output pins, A & B */
#define ENCODER_PIN_ENCA  AT91C_PIO_PB4
#define ENCODER_PIN_ENCB  AT91C_PIO_PB5

void encoder_initialize(void);

int encoder_get_counter(void);

void encoder_gearbox_shaft(const int encoder_counter,
			   bool *positive,
			   int *revolution,
			   uint16_t *position);

uint16_t encoder_get_gearbox_shaft_position(void);

/* ISR for encoder outputs A & B */
void encoder_isr_output_ab(void);

#endif /* __ENCODER_H__ */
