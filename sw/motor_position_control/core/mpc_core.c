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

#include "mpc_core.h"
#include "pid_ctrl.h"
#include "util.h"
#include "console.h"
#include "motor.h"
#include "adc.h"

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/
/* ADC=0-1023 ==> PWM=0-65535 */
#define ADC_DUTY_FACTOR  (64)

/* ADC=0-1023 ==> SHAFT=0-max */
#define ADC_POS_FACTOR ((float)motor_shaft_max_position() / 1023)

/* PID parameters */
#define P_GAIN  (100.0)
#define I_GAIN  (0.0)
#define D_GAIN  (0.0)

#define DUTY_MIN  (-65000.0)
#define DUTY_MAX  (65000.0)

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static struct pid_ctrl g_pid;

static bool g_pid_enable = false;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void mpc_core_initialize(void)
{
  /* disable PID controller */
  g_pid_enable = false;

  /* stop motor */
  motor_ctrl(true, 0);

  /* intialize PID controller */
  pid_ctrl_set_gain(&g_pid,
		    P_GAIN,
		    I_GAIN,
		    D_GAIN);

  pid_ctrl_set_output_limits(&g_pid,
			     DUTY_MIN,
			     DUTY_MAX);

  pid_ctrl_set_command_position(&g_pid,
				0.0);
}

/*****************************************************************/

void mpc_core_calibrate(bool zero_shaft)
{
  uint16_t shaft_position;
  const uint16_t max_shaft_position = motor_shaft_max_position();
  float command_pos;
  char hexstr[20];

  /* disable PID controller */
  g_pid_enable = false;

  /* stop motor */
  motor_ctrl(true, 0);

  /* set new origo */
  if (zero_shaft) {
    motor_zero_shaft_position();
  }

  /* get shaft position */
  shaft_position = motor_get_shaft_position();

  u16_hexstr(hexstr, shaft_position);
  console_put("CALIB: shaft:"); console_put(hexstr);
  u16_hexstr(hexstr, max_shaft_position);
  console_put(" / "); console_putln(hexstr);

  /* update PID set-point */
  command_pos = adc_get_value() * ADC_POS_FACTOR;
  pid_ctrl_set_command_position(&g_pid,
				command_pos);

  u16_hexstr(hexstr, (uint16_t)command_pos);
  console_put("CALIB: s-pos:"); console_putln(hexstr);
}

/*****************************************************************/

void mpc_core_position(void)
{
  float command_pos;
  char hexstr[20];

  /* enable PID controller */
  g_pid_enable = true;

  /* update PID set-point */
  command_pos = adc_get_value() * ADC_POS_FACTOR;
  pid_ctrl_set_command_position(&g_pid,
				command_pos);

  u16_hexstr(hexstr, (uint16_t)command_pos);
  console_put("POS: s-pos:"); console_putln(hexstr);
}

/*****************************************************************/

void mpc_core_tc0_callback(void)
{  
  static uint16_t cnt = 0;

  uint16_t shaft_pos;
  float pid_output;

  bool motor_clockwise;
  uint16_t motor_duty;  

  char hexstr[20];

  /* check if PID controller is enabled */
  if (!g_pid_enable) {
    return;
  } 
  
  /* ---------------------------------
   * PID controller loop
   * ---------------------------------
   */
  
  /* get current shaft position */
  shaft_pos = motor_get_shaft_position();

  /* update PID */
  pid_output = pid_ctrl_update(&g_pid, shaft_pos);

  /* apply new output */
  if (pid_output >= 0.0) {
    motor_clockwise = true;
    motor_duty = pid_output;   
  }
  else {
    motor_clockwise = false;
    motor_duty = (-1.0 * pid_output);    
  }

  motor_ctrl(motor_clockwise, motor_duty);
  
  /* this should happen at 1Hz, TC0 frequency is 100Hz (T=10ms) */
  if (++cnt >= 100) {
    console_put("TC0-CB-PID: ");
    if (motor_clockwise) {
      console_put("+");
    }
    else {
      console_put("-");
    }
    u16_hexstr(hexstr, motor_duty);
    console_putln(hexstr);

    cnt = 0;
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/
 
/*****************************************************************/
