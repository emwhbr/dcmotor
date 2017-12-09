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

#include <stdbool.h>

#include "mpc_fsm.h"
#include "mpc_core.h"
#include "console.h"
#include "led.h"

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static enum mpc_fsm_state g_current_state = MPC_FSM_STATE_INIT;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
static void execute_state_action(enum mpc_fsm_state state,
				 enum mpc_fsm_event event);

static void action_state_init(void);
static void action_state_calib(enum mpc_fsm_event event);
static void action_state_pos(void);

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void mpc_fsm_execute(enum mpc_fsm_event event)
{
  enum mpc_fsm_state next_state = g_current_state;

  /* determine next state */
  switch (g_current_state) {
  case MPC_FSM_STATE_INIT:
    switch (event) {      
    case MPC_FSM_EVENT_BUT_RELEASED:
      console_putln("INIT-->CALIB");
      next_state = MPC_FSM_STATE_CALIB;
      break;
    }
    break;
  case MPC_FSM_STATE_CALIB:
    switch (event) {      
    case MPC_FSM_EVENT_BUT_RELEASED:
      console_putln("CALIB-->POS");
      next_state = MPC_FSM_STATE_POS;
      break;
    }
    break;
  case MPC_FSM_STATE_POS:
    switch (event) {      
    case MPC_FSM_EVENT_BUT_RELEASED:
      console_putln("POS-->CALIB");
      next_state = MPC_FSM_STATE_CALIB;
      break;
    }
    break;
  }

  /* check if transition to new state */
  if (next_state != g_current_state) {
    g_current_state = next_state;
  }

  /* execute state action */
  execute_state_action(g_current_state, event);
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/
 
/*****************************************************************/

static void execute_state_action(enum mpc_fsm_state state,
				 enum mpc_fsm_event event)
{
  switch (state) {
  case MPC_FSM_STATE_INIT:
    action_state_init();
    break;
  case MPC_FSM_STATE_CALIB:
    action_state_calib(event);
    break;
  case MPC_FSM_STATE_POS:
    action_state_pos();
    break;
  }
}

/*****************************************************************/

static void action_state_init(void)
{
  /* signal state */
  led_on(LED_PIN_STAT);
  led_on(LED_PIN_PWR);

  /* initialize */
  mpc_core_initialize();
}

/*****************************************************************/

static void action_state_calib(enum mpc_fsm_event event)
{
  /* signal state */
  led_on(LED_PIN_STAT);
  led_off(LED_PIN_PWR);

  /* calibrate */
  mpc_core_calibrate(event == MPC_FSM_EVENT_BUT_PRESSED);
}

/*****************************************************************/

static void action_state_pos(void)
{
  static bool toggle = false;

  /* signal state : toggle LEDs */
  if (toggle) {
    led_on(LED_PIN_STAT);
    led_off(LED_PIN_PWR);
  }
  else {
    led_off(LED_PIN_STAT);
    led_on(LED_PIN_PWR);
  }
  toggle = !toggle;

  /* position control */
  mpc_core_position();  
}
