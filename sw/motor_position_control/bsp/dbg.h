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

#ifndef __DBG_H__
#define __DBG_H__

#include <stdint.h>

#include "sam9l9260.h"

/* debug output pins */
#define DBG_PIN_1  AT91C_PIO_PB2 /* external connector pin9 */
#define DBG_PIN_2  AT91C_PIO_PB3 /* external connector pin11 */

void dbg_initialize(void);

void dbg_pin_on(uint32_t pin);

void dbg_pin_off(uint32_t pin);

#endif /* __DBG_H__ */
