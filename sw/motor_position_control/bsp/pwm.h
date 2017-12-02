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

#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>

/* PWM duty control values */
#define PWM_DUTY_0   0x0000
#define PWM_DUTY_1   0x028d
#define PWM_DUTY_5   0x0ccc
#define PWM_DUTY_10  0x1999
#define PWM_DUTY_25  0x3fff
#define PWM_DUTY_50  0x7fff
#define PWM_DUTY_75  0xbfff
#define PWM_DUTY_90  0xe665
#define PWM_DUTY_99  0xfd6f
#define PWM_DUTY_100 0xffff

void pwm_initialize(void);

void pwm_set_duty(uint16_t duty_ctrl);

#endif /* __PWM_H__ */
