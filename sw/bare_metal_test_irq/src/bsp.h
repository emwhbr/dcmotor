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

#ifndef __BSP_H__
#define __BSP_H__

#include "sam9l9260.h"

/****************************************************************************
 *               BSP definitions
 ****************************************************************************/
/* master clock [Hz]*/
#define BSP_MCK  (204800000 / 2)   /* 102.400 MHz, configured in AT91Bootstrap */

/* system clock tick rate [Hz] */
#define BSP_TICKS_PER_SEC  (1000)

/* internal SRAM, first bank, addr:0x0020_0000 */
#define SRAM_ADDR  (AT91C_IRAM_1)

/* IRQ priorities */
#define ISR_BUT_PRIO  (AT91C_AIC_PRIOR_LOWEST + 0)
#define ISR_PIT_PRIO  (AT91C_AIC_PRIOR_LOWEST + 1)
#define ISR_TC0_PRIO  (AT91C_AIC_PRIOR_LOWEST + 2)
#define ISR_ENC_PRIO  (AT91C_AIC_PRIOR_LOWEST + 3)

/* Timer Counter - callback function */
typedef void (*tc_callback)(void);

/****************************************************************************
 *               BSP functions
 ****************************************************************************/
void bsp_high_level_init(tc_callback tc0_callback);

uint32_t bsp_get_pit_counter(void);

#endif /* __BSP_H__ */
