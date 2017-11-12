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

    u16_hexstr(hexstr, counter++);
    console_put("  cnt :");
    console_putln(hexstr);

    pit_counter = bsp_get_pit_cnt();
    u32_hexstr(hexstr, pit_counter);
    console_put("  pit :");
    console_putln(hexstr);

    /* toggle LEDs */
    led_on(SAM9L9260_STAT_LED);
    led_off(SAM9L9260_PWR_LED);
    delay();
    led_off(SAM9L9260_STAT_LED);
    led_on(SAM9L9260_PWR_LED);
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
