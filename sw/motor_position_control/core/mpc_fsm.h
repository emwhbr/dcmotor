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

#ifndef __MPC_FSM_H__
#define __MPC_FSM_H__

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/
/* state definitions */
enum mpc_fsm_state {
  MPC_FSM_STATE_INIT,  /* initialization   */
  MPC_FSM_STATE_CALIB, /* calibration      */
  MPC_FSM_STATE_POS,   /* position control */
  MPC_FSM_STATE_MAX
};

/* event definitions */
enum mpc_fsm_event {
  MPC_FSM_EVENT_NONE,
  MPC_FSM_EVENT_BUT_PRESSED,  /* button is pressed  */
  MPC_FSM_EVENT_BUT_RELEASED, /* button was pressed */
  MPC_FSM_EVENT_MAX
};

/****************************************************************************
 *               Exported functions
 ****************************************************************************/
void mpc_fsm_execute(enum mpc_fsm_event event);

#endif /* __MPC_FSM_H_*/
