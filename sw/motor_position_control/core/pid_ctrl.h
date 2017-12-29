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

#ifndef __PID_CTRL_H__
#define __PID_CTRL_H__

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/
/* pid controller "class" */
struct pid_ctrl {
  /* set-point */
  float m_command_position;

  /* error */
  float m_pos_error;

  /* output */  
  float m_output;
  float m_output_min;
  float m_output_max;

  /* proportional */
  float m_p_gain; /* (P)roportional gain */

  /* integral */
  float m_i_gain;  /* (I)ntegral gain                */
  float m_i_state; /* integrator state               */
  float m_i_min;   /* min allowable integrator state */
  float m_i_max;   /* max allowable integrator state */
  
  /* derivate */
  float m_d_gain;  /* (D)erivative gain   */
  float m_d_state; /* last position input */
};

/****************************************************************************
 *               Exported functions
 ****************************************************************************/
/* define PID parameters */
void pid_ctrl_initialize(struct pid_ctrl *pid,
			 float p_gain,
			 float i_gain,
			 float d_gain);

/* output limits */
void pid_ctrl_set_output_limits(struct pid_ctrl *pid,
				float min,
				float max);

/* update set-point */
void pid_ctrl_set_command_position(struct pid_ctrl *pid,
				   float value);

/* performs the PID calculation*/
float pid_ctrl_update(struct pid_ctrl *pid,
		      float position);

#endif /* __PID_CTRL_H_*/
