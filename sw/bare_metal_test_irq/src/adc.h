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

#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

#include "sam9l9260.h"

/* ADC input pin */
#define ADC_PIN_INPUT  AT91C_PC0_AD0 /* AD0 (PC0) */

void adc_initialize(void);

uint16_t adc_get_value(void);

#endif /* __ADC_H__ */
