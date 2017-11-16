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

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "sam9l9260.h"

/* encoder output pins, A & B */
#define ENCODER_PIN_ENCA  AT91C_PIO_PB4
#define ENCODER_PIN_ENCB  AT91C_PIO_PB5

void encoder_initialize(void);

int encoder_get_counter(void);

/* ISR for encoder outputs A & B */
void encoder_isr_output_ab(void);

#endif /* __ENCODER_H__ */
