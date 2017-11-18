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
#include "util.h"

/****************************************************************************
 *               Types and definitions
 ****************************************************************************/
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

/****************************************************************************
 *               Global variables
 ****************************************************************************/
/* TBD: adding globals here make system behave funny!!! */

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
static void test_bsp_pit(uint32_t *pit_counter_delta);
static void test_button(void);
static void test_encoder(const uint32_t *pit_counter_delta);
static void test_motor(void);

static void delay(void);

/*****************************************************************/

void c_main()  /* main() */
{
  uint16_t counter = 0;

  uint32_t *mem_ptr;
  uint32_t mem_val;
  
  uint32_t pit_counter_delta;    

  char hexstr[20];

  /* initialize BSP */
  bsp_high_level_init();

  console_putln("\nBare metal IRQ alive!");

  /* examine address 0 / sram */
  console_putln("----- ADDR : 0x0 -----");
  mem_ptr = (uint32_t *) 0;
  for (int i=0; i < 16; i++) {
    mem_val = *mem_ptr;
    mem_ptr++;
    u32_hexstr(hexstr, mem_val);
    console_put("  mem :");
    console_putln(hexstr);
  }
  console_putln("----- SRAM -----");
  mem_ptr = (uint32_t *) SRAM_ADDR;
  for (int i=0; i < 16; i++) {
    mem_val = *mem_ptr;
    mem_ptr++;
    u32_hexstr(hexstr, mem_val);
    console_put("  sram :");
    console_putln(hexstr);
  }

  /* 
   * toggle green and yellow led
   * during IRQ test
   */
  while (1) {
    console_putln("----------");

    /* update local counter */
    u16_hexstr(hexstr, counter++);
    console_put("  cnt :"); console_putln(hexstr);

    /* test BSP PIT */
    test_bsp_pit(&pit_counter_delta);

    /* test BUTTON */
    test_button();

    /* test ENCODER */
    test_encoder(&pit_counter_delta);

    /* test MOTOR */
    test_motor();    

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

static void test_bsp_pit(uint32_t *pit_counter_delta)
{
  uint32_t pit_counter;
  static uint32_t pit_counter_old = 0; 
  uint32_t time_ms;
  uint32_t time_ms_delta;

  char hexstr[20];

  /* get BSP PIT counter */
  pit_counter = bsp_get_pit_counter();
  *pit_counter_delta = pit_counter - pit_counter_old;
  pit_counter_old = pit_counter;
  
  u32_hexstr(hexstr, pit_counter);
  console_put("  pit :"); console_put(hexstr);
  u32_hexstr(hexstr, *pit_counter_delta);
  console_put("  diff:"); console_putln(hexstr);
  
  /* get absolute time in milliseconds */
  time_ms = pit_counter * 1000 / BSP_TICKS_PER_SEC;
  time_ms_delta = *pit_counter_delta * 1000 / BSP_TICKS_PER_SEC;
  
  u32_hexstr(hexstr, time_ms);
  console_put("  msec:"); console_put(hexstr);
  u32_hexstr(hexstr, time_ms_delta);
  console_put("  diff:"); console_putln(hexstr);  
}

/*****************************************************************/

static void test_button(void)
{
  /* get button */
  console_put("  butt:");
  if (button_is_pressed()) {
    console_put("PRESSED    ");
  }
  else {
    console_put("NOT PRESSED");
  }
  console_put(", was :");
  if (button_was_pressed()) {
    console_putln("PRESSED    ");
  }
  else {
    console_putln("NOT PRESSED");
  }
}

/*****************************************************************/

static void test_encoder(const uint32_t *pit_counter_delta)
{ 
  static int encoder_counter_old = 0;
  int encoder_counter;
  int encoder_counter_delta;
  
  float encoder_freq_f;
  uint32_t encoder_freq;

  bool gearbox_shaft_clockwise;
  int gearbox_shaft_rev;
  uint16_t gearbox_shaft_pos;

  char hexstr[20];

  /* get encoder counter */
  encoder_counter = encoder_get_counter();
  encoder_counter_delta = encoder_counter - encoder_counter_old;
  encoder_counter_old = encoder_counter;
  
  encoder_freq_f = (encoder_counter_delta / 4.0f) / 
    (*pit_counter_delta * (1.0f / BSP_TICKS_PER_SEC) );
  encoder_freq = (uint32_t) encoder_freq_f;
  
  u32_hexstr(hexstr, encoder_counter);
  console_put("  enc :"); console_put(hexstr);
  u32_hexstr(hexstr, encoder_counter_delta);
  console_put("  diff:"); console_put(hexstr);
  u32_hexstr(hexstr, encoder_freq);
  console_put("  freq:"); console_putln(hexstr);

  /* gearbox output shaft position */
  encoder_get_gearbox_shaft(encoder_counter,
			    &gearbox_shaft_clockwise,
			    &gearbox_shaft_rev,
			    &gearbox_shaft_pos);
  u32_hexstr(hexstr, gearbox_shaft_rev);
  if (gearbox_shaft_clockwise) {
    console_put("  rev :(POS)"); console_put(hexstr);
  }
  else {
    console_put("  rev :(NEG)"); console_put(hexstr);
  }
  u16_hexstr(hexstr, gearbox_shaft_pos);
  console_put("  pos:"); console_putln(hexstr);
}

/*****************************************************************/

static void test_motor(void)
{
  bool motor_ctrl_clockwise;
  uint16_t motor_ctrl_duty = PWM_DUTY_25;

  if (button_is_pressed()) {
    motor_ctrl_clockwise = false;
  }
  else {
    motor_ctrl_clockwise = true;
  }

  /* motor control */
  motor_ctrl(motor_ctrl_clockwise,
	     motor_ctrl_duty);
}

/*****************************************************************/

static void delay(void)
{
  int i, j;
  for(i = 0; i < 369000; i++) j += i;
}
