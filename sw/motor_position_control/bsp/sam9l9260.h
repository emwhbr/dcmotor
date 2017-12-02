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

#ifndef __SAM9L9260_H__
#define __SAM9L9260_H__

#include <stdint.h>

#include "at91sam9260.h"

/****************************************************************************
 *               SAM9-L9260 Development Board (Olimex)
 ****************************************************************************/
#define BIT(X)  ( (uint32_t) (1 << (X)) )

/* LED definitions */
#define SAM9L9260_PIN_LED_STAT  BIT(6) /* pin controlled by PA6 (green)  */
#define SAM9L9260_PIN_LED_PWR   BIT(9) /* pin controlled by PA9 (yellow) */

/* user button */
#define SAM9L9260_PIN_USER_BUTTON  BIT(15) /* pin controlled by PC15 */

#endif /* __SAM9L9260_H__ */
