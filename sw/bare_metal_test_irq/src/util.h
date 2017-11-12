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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>

void u8_hexstr(char *str,
	       uint8_t val);

void u16_hexstr(char *str,
		uint16_t val);

void u32_hexstr(char *str,
		uint32_t val);

#endif /* __UTIL_H__ */
