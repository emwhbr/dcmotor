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

#include "console.h"
#include "sam9l9260.h"

/****************************************************************************
 *               Global variables
 ****************************************************************************/

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
void console_write_char(char c);

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void console_initialize(void)
{
  /* initialize the debug port */
  AT91C_BASE_DBGU->DBGU_CR = 0b0000000001010000;
  AT91C_BASE_DBGU->DBGU_MR = 0b0000100000000000;
}

/*****************************************************************/

void console_put(const char *str)
{
  for (int i = 0; str[i] != '\0'; i++) {
    console_write_char(str[i]);
  }
}

/*****************************************************************/

void console_putln(const char *str)
{
  console_put(str);
  console_put("\n");
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

void console_write_char(char c)
{
  /* transmit one character on the debug port,
   * wait for debug port to be ready to transmit
   */
  while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
  AT91C_BASE_DBGU->DBGU_THR = c;

  /* append carriage return to new line */
  if (c == '\n') {
    while ( !(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) );
    AT91C_BASE_DBGU->DBGU_THR = '\r';
  }
}
