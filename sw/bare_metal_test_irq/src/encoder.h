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

void encoder_initialize(void);

int encoder_get_counter(void);

/* ISR for encoder output A */
void encoder_isr_output_a(void);

#endif /* __ENCODER_H__ */
