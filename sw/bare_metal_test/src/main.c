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

#include <stdint.h>

#include "console.h"
#include "led.h"
#include "pwm.h"
#include "sam9l9260.h"

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/
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

/****************************************************************************
 *               Global variables
 ****************************************************************************/
/* TBD: adding globals here make system behave funny!!! */

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
static void delay();

static void uint16_2_hexstr(char *str,
			    uint16_t data);

/*****************************************************************/

void c_entry()  /* main() */
{
  uint16_t pwm_duty_ctrl_0 = PWM_DUTY_75;
  uint16_t pwm_duty_ctrl_1 = PWM_DUTY_10;
  uint16_t pwm_duty_ctrl_2 = PWM_DUTY_90;

  char hexstr[10];

  /* initialize hardware */
  console_initialize();  
  led_initialize();
  pwm_initialize();

  console_putln("\nBare metal PWM alive!");

  /* set pwm duty */
  pwm_set_duty(pwm_duty_ctrl_0);

  uint16_2_hexstr(hexstr, pwm_duty_ctrl_0);
  console_put("duty0:");
  console_putln(hexstr);

  /* toggle green and yellow led */
  while (1) {
    pwm_set_duty(pwm_duty_ctrl_1);
    uint16_2_hexstr(hexstr, pwm_duty_ctrl_1);
    console_put("duty1:");
    console_putln(hexstr);

    led_on(SAM9L9260_STAT_LED);
    led_off(SAM9L9260_PWR_LED);

    delay();

    pwm_set_duty(pwm_duty_ctrl_2);
    uint16_2_hexstr(hexstr, pwm_duty_ctrl_2);
    console_put("duty2:");
    console_putln(hexstr);

    led_off(SAM9L9260_STAT_LED);
    led_on(SAM9L9260_PWR_LED);
    delay();
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

static void delay()
{
  int i, j;
  for(i = 0; i < 400000; i++) j += i;
}

/*****************************************************************/

static void uint16_2_hexstr(char *str,
			    uint16_t data)
{
  /* we make our string assuming all hex digits are 0 to 9
   * string will be of the form 0xabcd
   * where a,b,c,d are the individual hex digits
   */
  str[0] = '0';
  str[1] = 'x';
  str[2] = ( (data>>12) & 0x0F) + '0';
  str[3] = ( (data>>8)  & 0x0F) + '0';
  str[4] = ( (data>>4)  & 0x0F) + '0';
  str[5] = ( (data)     & 0x0F) + '0';
  str[6] = 0;  
 
  /* now we correct for the case where a digit
   * is A to F:
   */
  if (str[2] > '9') {str[2] += 7;}
  if (str[3] > '9') {str[3] += 7;}
  if (str[4] > '9') {str[4] += 7;}
  if (str[5] > '9') {str[5] += 7;}
}  
