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

#include "pid_ctrl.h"

/*
 * Implementation notes:
 * 1. PID without a PhD, Tim Wescott
 *    http://m.eet.com/media/1112634/f-wescot.pdf
 *
 * 2. Arduino PID library
 *    http://playground.arduino.cc/Code/PIDLibrary
 */

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void pid_ctrl_initialize(struct pid_ctrl *pid,
			 float p_gain,
			 float i_gain,
			 float d_gain)
{
  /* default output range: 0 - 100 */
  pid_ctrl_set_command_position(pid, 0.0);
  pid_ctrl_set_output_limits(pid, 0.0, 100.0);

  pid->m_pos_error = 0.0;
  
  pid->m_output = pid->m_output_min;

  pid->m_p_gain = p_gain;

  pid->m_i_gain = i_gain;
  pid->m_i_state = 0.0;

  pid->m_d_gain = d_gain;  
  pid->m_d_state = pid->m_command_position;
}

/*****************************************************************/

void pid_ctrl_set_output_limits(struct pid_ctrl *pid,
				float min,
				float max)
{
  pid->m_output_min = min;
  pid->m_output_max = max;

  pid->m_i_min = min;
  pid->m_i_max = max;
}

/*****************************************************************/

void pid_ctrl_set_command_position(struct pid_ctrl *pid,
				   float value)
{
  pid->m_command_position = value;
}

/*****************************************************************/

float pid_ctrl_update(struct pid_ctrl *pid,
		      float position)
{
  float p_term;
  float i_term;
  float d_term;

  float output;

  /* calculate error */
  pid->m_pos_error = pid->m_command_position - position;

  /* calculate PROPORTIONAL term */
  p_term = pid->m_p_gain * pid->m_pos_error;

  /* Calculate INTEGRAL term */
  pid->m_i_state += pid->m_pos_error;
  if (pid->m_i_state > pid->m_i_max) {
    pid->m_i_state = pid->m_i_max;
  }
  else if (pid->m_i_state < pid->m_i_min) {
    pid->m_i_state = pid->m_i_min;
  }
  i_term = pid->m_i_gain * pid->m_i_state;

  /* calculate DERIVATE term */
  d_term = pid->m_d_gain * (pid->m_d_state - position);
  pid->m_d_state = position;

  /* limit output */
  output = p_term + i_term + d_term;
  if (output > pid->m_output_max) {
    output = pid->m_output_max;
  }
  else if (output < pid->m_output_min) {
    output = pid->m_output_min;
  }
  pid->m_output = output;

  return output;
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/
 
/*****************************************************************/
