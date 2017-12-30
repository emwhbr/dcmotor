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
#include "dbg.h"

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/
/* ADC=0-1023 ==> PWM=0-65535 */
#define ADC_DUTY_FACTOR  (64)

/* ADC=0-1023 ==> SHAFT=0-max */
#define ADC_POS_FACTOR ((float)motor_shaft_max_position() / 1023)

/* PID parameters */
#define P_GAIN  (300.0)
#define I_GAIN  (0.0)
#define D_GAIN  (1650.0)

#define DUTY_MIN  (-65000.0)
#define DUTY_MAX  ( 65000.0)

/* motor control state definitions */
enum motor_ctrl_state {
  MOTOR_CTRL_STATE_RESET,
  MOTOR_CTRL_STATE_CLOCKWISE,
  MOTOR_CTRL_STATE_ANTICLOCKWISE,
  MOTOR_CTRL_STATE_STOP
};

/* forced number of times in stop-state
 * to avoid fast changes of motor direction,
 * absolute time = N x 5ms = 150ms
 */
#define MOTOR_STOP_CNT_MAX  (30)

/* moving average for commanded position,
 * absolute time = N x 5ms = 50ms
 */
#define COMMAND_POS_WINDOW_SIZE  (10)

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static struct pid_ctrl g_pid;
static bool g_pid_enable = false;

static enum motor_ctrl_state g_current_motor_ctrl_state = MOTOR_CTRL_STATE_RESET;
static uint16_t g_motor_stop_cnt = 0;

static float g_command_pos_window[COMMAND_POS_WINDOW_SIZE] = {0.0,
							      0.0,
							      0.0,
							      0.0,
							      0.0};
static int g_command_pos_idx = 0;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
float get_command_pos_moving_average(void);

void motor_ctrl_state(float pid_output,
		      bool *motor_clockwise,
		      uint16_t *motor_duty);

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
  pid_ctrl_initialize(&g_pid,
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

  /* re-initialize PID controller */
  pid_ctrl_initialize(&g_pid,
		      P_GAIN,
		      I_GAIN,
		      D_GAIN);

  pid_ctrl_set_output_limits(&g_pid,
			     DUTY_MIN,
			     DUTY_MAX);

  /* update PID set-point */
  command_pos = get_command_pos_moving_average();
  pid_ctrl_set_command_position(&g_pid,
				command_pos);

  u16_hexstr(hexstr, (uint16_t)command_pos);
  console_put("CALIB: s-pos:"); console_putln(hexstr);

  /* DEBUG: clear all */
  dbg_pin_off(DBG_PIN_1);
  dbg_pin_off(DBG_PIN_2);
}

/*****************************************************************/

void mpc_core_position(void)
{
  /* enable PID controller */
  g_pid_enable = true;

  console_putln("POS: on");
}

/*****************************************************************/

void mpc_core_tc0_callback(void)
{  
  uint16_t shaft_pos;
  float pid_output;

  bool motor_clockwise;
  uint16_t motor_duty;

  static bool dbg_first = true;
  static int dbg_cnt = 0;
  float dbg_pos_error;
  char hexstr[20];

  /* check if PID controller is enabled */
  if (!g_pid_enable) {
    g_current_motor_ctrl_state = MOTOR_CTRL_STATE_RESET;
    g_motor_stop_cnt = 0;
    dbg_first = true;
    return;
  }

  /* DEBUG: create pulse first time */
  if (dbg_first) {
    dbg_first = false;
     dbg_pin_on(DBG_PIN_1);
     dbg_pin_off(DBG_PIN_1);
  }
  
  /* ---------------------------------
   * PID controller loop
   * ---------------------------------
   */
  
  /* get current shaft position */
  shaft_pos = motor_get_shaft_position();

  /* update PID */
  pid_output = pid_ctrl_update(&g_pid, shaft_pos);

  /* calculate new motor output, limited if necessary  */
  motor_ctrl_state(pid_output,
		   &motor_clockwise,
		   &motor_duty);

  /* apply new output */
  motor_ctrl(motor_clockwise, motor_duty);

  /* DEBUG: every cycle */
  dbg_pos_error = g_pid.m_pos_error;
  if (dbg_pos_error < 0.0) {
    dbg_pos_error *= -1.0;
  }
  if (dbg_pos_error < 25) {
    dbg_pin_on(DBG_PIN_1); /* within +- 25 points, +-1.5% */
  }
  else {
    dbg_pin_off(DBG_PIN_1);
  }

  /* DEBUG: every 50ms */
  if (++dbg_cnt == 10) {
    if (motor_clockwise) {
      u16_hexstr(hexstr, (uint16_t)pid_output);
      console_put("TC0: pid:+"); console_putln(hexstr);
    }
    else {
      u16_hexstr(hexstr, (uint16_t)(pid_output * -1.0));
      console_put("TC0: pid:-"); console_putln(hexstr);
    }
    dbg_cnt = 0;
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

 /*****************************************************************/

float get_command_pos_moving_average(void)
{
  int i;
  float command_pos;
  float avg = 0.0;

  command_pos = adc_get_value() * ADC_POS_FACTOR;

  g_command_pos_window[g_command_pos_idx++] = command_pos;
  if (g_command_pos_idx >= COMMAND_POS_WINDOW_SIZE) {
    g_command_pos_idx = 0;
  }

  for (i=0; i < COMMAND_POS_WINDOW_SIZE; i++) {
    avg += g_command_pos_window[i];
  }

  return (avg / COMMAND_POS_WINDOW_SIZE);
}

/*****************************************************************/

void motor_ctrl_state(float pid_output,
		      bool *motor_clockwise,
		      uint16_t *motor_duty)
{
  enum motor_ctrl_state next_state = g_current_motor_ctrl_state;

  /* determine motor duty and direction */
  if (pid_output >= 0.0) {
    *motor_clockwise = true;
    *motor_duty = pid_output;   
  }
  else {
    *motor_clockwise = false;
    *motor_duty = (-1.0 * pid_output);    
  }

  /* determine next state and limit output if necessary */
  switch (g_current_motor_ctrl_state) {
  case MOTOR_CTRL_STATE_RESET:
    if (*motor_clockwise) {
      next_state = MOTOR_CTRL_STATE_CLOCKWISE;
      console_putln("CW");
    }
    else {
      next_state = MOTOR_CTRL_STATE_ANTICLOCKWISE;
      console_putln("ACW");
    }
    break;
  case MOTOR_CTRL_STATE_CLOCKWISE:
    /* changed direction requires stop-state */
    if (*motor_clockwise == false) {
      *motor_clockwise = true;
      *motor_duty = *motor_duty / 2;
      next_state = MOTOR_CTRL_STATE_STOP;
      console_putln("CW-->STOP");
      dbg_pin_on(DBG_PIN_2);
    }
    break;
  case MOTOR_CTRL_STATE_ANTICLOCKWISE:
    /* changed direction requires stop-state */
    if (*motor_clockwise == true) {
      *motor_clockwise = false;
      *motor_duty = *motor_duty / 2;
      next_state = MOTOR_CTRL_STATE_STOP;
      console_putln("ACW-->STOP");
      dbg_pin_on(DBG_PIN_2);
    }
    break;
  case MOTOR_CTRL_STATE_STOP:
    *motor_clockwise = true;
    *motor_duty = 0;
    if (++g_motor_stop_cnt >= MOTOR_STOP_CNT_MAX) {
      g_motor_stop_cnt = 0;
      next_state = MOTOR_CTRL_STATE_RESET;
      dbg_pin_off(DBG_PIN_2);
    }
    break;
  }

  /* check if transition to new state */
  if (next_state != g_current_motor_ctrl_state) {
    g_current_motor_ctrl_state = next_state;
  }
}
