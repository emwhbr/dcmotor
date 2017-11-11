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

#include <stdint.h>

#include "console.h"
#include "led.h"
#include "irq.h"
#include "sam9l9260.h"

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

static void uint16_2_hexstr(char *str,
			    uint16_t data);

/*****************************************************************/

void c_main()  /* main() */
{
  uint16_t counter = 0;

  uint16_t *mem_ptr;
  uint16_t mem_val;

  uint16_t pit_counter;

  char hexstr[10];

  console_initialize();
  console_putln("\nBare metal IRQ alive!");

  /* setup interrupt handling */
  irq_initialize();

  /* initialize hardware */
  led_initialize();

  console_putln("IRQ initialize done");

  console_putln("----- ADDR : 0x0 -----");
  mem_ptr = (uint16_t *) 0;
  for (int i=0; i < 32; i++) {
    mem_val = *mem_ptr;
    mem_ptr++;
    uint16_2_hexstr(hexstr, mem_val);
    console_put("  mem :");
    console_putln(hexstr);
  }
  console_putln("----- SRAM -----");
  mem_ptr = (uint16_t *) 0x200000;
  for (int i=0; i < 32; i++) {
    mem_val = *mem_ptr;
    mem_ptr++;
    uint16_2_hexstr(hexstr, mem_val);
    console_put("  sram :");
    console_putln(hexstr);
  }


  /* toggle green and yellow led
   * during IRQ test */
  while (1) {
    console_putln("----------");

    uint16_2_hexstr(hexstr, counter++);
    console_put("  cnt :");
    console_putln(hexstr);

    pit_counter = irq_get_pit_cnt();
    uint16_2_hexstr(hexstr, pit_counter);
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
  for(i = 0; i < 400000; i++) j += i;
}

/*****************************************************************/

static void uint16_2_hexstr(char *str,
			    uint16_t data)
{
  /* we make our string assuming all hex digits are 0 to 9
   * string will be of the form 0xabcd
   * where a,b,c,d are the individual hex digits
   */
  str[0] = '0';
  str[1] = 'x';
  str[2] = ( (data>>12) & 0x0F) + '0';
  str[3] = ( (data>>8)  & 0x0F) + '0';
  str[4] = ( (data>>4)  & 0x0F) + '0';
  str[5] = ( (data)     & 0x0F) + '0';
  str[6] = 0;  
 
  /* now we correct for the case where a digit
   * is A to F:
   */
  if (str[2] > '9') {str[2] += 7;}
  if (str[3] > '9') {str[3] += 7;}
  if (str[4] > '9') {str[4] += 7;}
  if (str[5] > '9') {str[5] += 7;}
}  
