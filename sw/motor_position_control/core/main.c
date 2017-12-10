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

#include "bsp.h"
#include "console.h"
#include "button.h"
#include "util.h"
#include "mpc_product_info.h"
#include "mpc_fsm.h"
#include "mpc_core.h"

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/

/****************************************************************************
 *               Global variables
 ****************************************************************************/
/* TBD: adding globals here make system behave funny!!! */

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
static enum mpc_fsm_event check_user_button(void);
static void delay_10x_ms(uint8_t x);

/*****************************************************************/

void c_main()  /* main() */
{
  enum mpc_fsm_event event = MPC_FSM_EVENT_NONE;

  /* initialize BSP */
  bsp_high_level_init(mpc_core_tc0_callback);

  console_put("\n");
  console_put(MPC_PRODUCT_NUMBER);
  console_put("-");
  console_putln(MPC_RSTATE);

  /* 
   * execute state machine
   */
  while (1) {
    /* update state machine */
    event = check_user_button();;
    mpc_fsm_execute(event);    

    /* take it easy, 100ms */    
    delay_10x_ms(10);
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

static enum mpc_fsm_event check_user_button(void)
{
  /* check state of user button */
  if (button_is_pressed()) {
    return MPC_FSM_EVENT_BUT_PRESSED;
  }
  else if (button_was_pressed()) {
    return MPC_FSM_EVENT_BUT_RELEASED;
  }
  else {
    return MPC_FSM_EVENT_NONE;
  }
}

/*****************************************************************/

static void delay_10x_ms(uint8_t x)
{
  const int n = x * 17857; /* 10ms = 17857 */
  int i, j;

  for(i = 0; i < n; i++) j += i;
}
