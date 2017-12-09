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

#ifndef __MPC_CORE_H__
#define __MPC_CORE_H__

#include <stdbool.h>

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

void mpc_core_initialize(void);

void mpc_core_calibrate(bool zero_shaft);

void mpc_core_position(void);

/*
 * This is the actual PID controller.
 * Executed as a callback to TC0 at 100Hz (T=10ms).
 */
void mpc_core_tc0_callback(void);

#endif /* __MPC_CORE_H_*/
