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

#include "util.h"

/****************************************************************************
 *               Global variables
 ****************************************************************************/

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void u8_hexstr(char *str,
	       uint8_t val)
{
  /* we make our string assuming all hex digits are 0 to 9
   * string will be of the form 0xabcd
   * where a,b,c,d are the individual hex digits
   */
  str[0] = '0';
  str[1] = 'x';
  str[2] = ( (val>>4)  & 0x0F) + '0';
  str[3] = ( (val)     & 0x0F) + '0';
  str[4] = 0;  
 
  /* now we correct for the case where a digit
   * is A to F:
   */
  if (str[2] > '9') {str[2] += 7;}
  if (str[3] > '9') {str[3] += 7;}
}

/*****************************************************************/

void u16_hexstr(char *str,
		uint16_t val)
{
  /* we make our string assuming all hex digits are 0 to 9
   * string will be of the form 0xabcd
   * where a,b,c,d are the individual hex digits
   */
  str[0] = '0';
  str[1] = 'x';

  str[2] = ( (val>>12) & 0x0F) + '0';
  str[3] = ( (val>>8)  & 0x0F) + '0';

  str[4] = ( (val>>4)  & 0x0F) + '0';
  str[5] = ( (val)     & 0x0F) + '0';

  str[6] = 0;  
 
  /* now we correct for the case where a digit
   * is A to F:
   */
  if (str[2] > '9') {str[2] += 7;}
  if (str[3] > '9') {str[3] += 7;}
  if (str[4] > '9') {str[4] += 7;}
  if (str[5] > '9') {str[5] += 7;}
}

/*****************************************************************/

void u32_hexstr(char *str,
		uint32_t val)
{
  /* we make our string assuming all hex digits are 0 to 9
   * string will be of the form 0xabcd
   * where a,b,c,d are the individual hex digits
   */
  str[0] = '0';
  str[1] = 'x';

  str[2] = ( (val>>28) & 0x0F) + '0';
  str[3] = ( (val>>24) & 0x0F) + '0';

  str[4] = ( (val>>20) & 0x0F) + '0';
  str[5] = ( (val>>16) & 0x0F) + '0';

  str[6] = ( (val>>12) & 0x0F) + '0';
  str[7] = ( (val>>8)  & 0x0F) + '0';

  str[8] = ( (val>>4)  & 0x0F) + '0';
  str[9] = ( (val)     & 0x0F) + '0';

  str[10] = 0;
 
  /* now we correct for the case where a digit
   * is A to F:
   */
  if (str[2] > '9') {str[2] += 7;}
  if (str[3] > '9') {str[3] += 7;}
  if (str[4] > '9') {str[4] += 7;}
  if (str[5] > '9') {str[5] += 7;}
  if (str[6] > '9') {str[6] += 7;}
  if (str[7] > '9') {str[7] += 7;}
  if (str[8] > '9') {str[8] += 7;}
  if (str[9] > '9') {str[9] += 7;}
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/
 
/*****************************************************************/
