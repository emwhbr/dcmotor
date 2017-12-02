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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdbool.h>

#include "sam9l9260.h"

/* user button pin */
#define BUTTON_PIN_USER_BUTTON  SAM9L9260_PIN_USER_BUTTON

void button_initialize(void);

bool button_is_pressed(void);

bool button_was_pressed(void);

/* ISR for user button pressed */
void button_isr_pressed(void);

#endif /* __BUTTON_H__ */
