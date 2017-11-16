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

#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

/* led pins */
#define LED_PIN_STAT  SAM9L9260_PIN_LED_STAT
#define LED_PIN_PWR   SAM9L9260_PIN_LED_PWR

void led_initialize(void);

void led_on(uint32_t pin_led);

void led_off(uint32_t pin_led);

#endif /* __LED_H__ */
