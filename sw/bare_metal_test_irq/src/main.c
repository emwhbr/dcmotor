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

#include "bsp.h"
#include "console.h"
#include "led.h"
#include "button.h"
#include "encoder.h"
#include "util.h"

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
static void delay();

/*****************************************************************/

void c_main()  /* main() */
{
  uint16_t counter = 0;

  uint32_t *mem_ptr;
  uint32_t mem_val;

  uint32_t pit_counter;
  uint32_t pit_counter_old = 0;
  uint32_t pit_counter_delta;

  int encoder_counter;
  int encoder_counter_old = 0;
  int encoder_counter_delta;

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

  /* toggle green and yellow led
   * during IRQ test */
  while (1) {
    console_putln("----------");

    /* update local counter */
    u16_hexstr(hexstr, counter++);
    console_put("  cnt :"); console_putln(hexstr);

    /* get BSP PIT counter */
    pit_counter = bsp_get_pit_counter();
    pit_counter_delta = pit_counter - pit_counter_old;
    pit_counter_old = pit_counter;

    u32_hexstr(hexstr, pit_counter);
    console_put("  pit :"); console_put(hexstr);
    u32_hexstr(hexstr, pit_counter_delta);
    console_put("  diff:"); console_putln(hexstr);

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

    /* get encoder counter */
    encoder_counter = encoder_get_counter();
    encoder_counter_delta = encoder_counter - encoder_counter_old;
    encoder_counter_old = encoder_counter;

    u32_hexstr(hexstr, encoder_counter);
    console_put("  enc :"); console_put(hexstr);
    u32_hexstr(hexstr, encoder_counter_delta);
    console_put("  diff:"); console_putln(hexstr);

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

static void delay()
{
  int i, j;
  for(i = 0; i < 379000; i++) j += i;
}
