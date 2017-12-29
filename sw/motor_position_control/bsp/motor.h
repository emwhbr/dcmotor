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

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include <stdbool.h>

#include "sam9l9260.h"

/* motor direction control pin */
#define MOTOR_PIN_DIR  AT91C_PIO_PB1 /* external connector pin7 */

void motor_initialize();

void motor_ctrl(bool clockwise,
		uint16_t duty);

void motor_zero_shaft_position(void);

uint16_t motor_get_shaft_position(void);

uint16_t motor_shaft_max_position(void);

#endif /* __MOTOR_H__ */
