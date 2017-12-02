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
#include "led.h"
#include "button.h"
#include "motor.h"
#include "encoder.h"
#include "adc.h"
#include "util.h"
#include "mpc_product_info.h"

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/
/* ADC=0-1023 ==> PWM=0-65535 */
#define ADC_DUTY_FACTOR  64

/****************************************************************************
 *               Global variables
 ****************************************************************************/
/* TBD: adding globals here make system behave funny!!! */

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
static void test_tc0_callback(void);
static void delay(void);

/*****************************************************************/

void c_main()  /* main() */
{
  uint16_t counter = 0;

  bool motor_ctrl_clockwise;
  uint16_t motor_ctrl_duty;

  char hexstr[20];

  /* initialize BSP */
  bsp_high_level_init(test_tc0_callback);

  console_put("\n");
  console_put(MPC_PRODUCT_NUMBER);
  console_put("-");
  console_putln(MPC_RSTATE);

  /* 
   * toggle green and yellow led
   */
  while (1) {
    console_putln("----------");

    /* update local counter */
    u16_hexstr(hexstr, counter++);
    console_put("  cnt :"); console_putln(hexstr);

    /* motor control */
    if (button_is_pressed()) {
      motor_ctrl_clockwise = false;
    }
    else {
      motor_ctrl_clockwise = true;
    }

    motor_ctrl_duty = adc_get_value() * ADC_DUTY_FACTOR;

    motor_ctrl(motor_ctrl_clockwise,
	       motor_ctrl_duty);

    /* toggle LEDs */
    led_on(LED_PIN_STAT);
    led_off(LED_PIN_PWR);    
    delay();
    led_off(LED_PIN_STAT);
    led_on(LED_PIN_PWR);
    delay();    
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

static void test_tc0_callback(void)
{
  static uint16_t cnt = 0;

  // this should happen at 0.5Hz, TC0 frequency is 100Hz (T=10ms)
  if (++cnt >= 200) {
    console_putln("TC0-CBF");
    cnt = 0;
  }
}

/*****************************************************************/

static void delay(void)
{
  int i, j;
  for(i = 0; i < 369000; i++) j += i;
}
