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

#include "at91sam9260.h"

/****************************************************************************
 *               SAM9-L9260 Development Board (Olimex)
 ****************************************************************************/

#define BIT(X)  (1 << (X))

/* LED definitions */
#define SAM9L9260_STAT_LED  BIT(6) /* pin controlled by PA6 (green)  */
#define SAM9L9260_PWR_LED   BIT(9) /* pin controlled by PA9 (yellow) */

#endif /* __SAM9L9260_H__ */
