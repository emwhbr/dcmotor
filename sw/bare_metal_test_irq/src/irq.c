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

#include "irq.h"
#include "startup.h"
#include "arm_exc.h"
#include "console.h"
#include "sam9l9260.h"

/****************************************************************************
 *               Macro definitions
 ****************************************************************************/
/* master clock [Hz]*/
#define MCK  (204800000 / 2)   /* 102.400 MHz, configured in at91bootstrap */

/* system clock tick rate [Hz] */
#define BSP_TICKS_PER_SEC  100

/* IRQ priorities */
#define ISR_PIT_PRIO  (AT91C_AIC_PRIOR_LOWEST + 0)

/****************************************************************************
 *               Global variables
 ****************************************************************************/
volatile AT91S_MATRIX *g_matrix = (AT91S_MATRIX *) AT91C_BASE_MATRIX;

static volatile uint16_t g_pit_cnt = 1972;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
void irq_isr_bsp_spur(void);
void irq_isr_bsp_pit(void);

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void irq_initialize(void)
{
  uint32_t piv;

  const uint32_t LDR_PC_PC = 0xe59ff000; /* contains the opcode of ARM
					  * instruction LDR pc,[pc,...],
					  * which is used to populate
					  * SRAM vector table
					  */

  const uint32_t MAGIC = 0xdeadbeef; /* used to test if remap operation
				      * has been performed already
				      */

  /*
   * ARM exception vectors
   */
  /* setup the primary vector table in SRAM */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x00) = LDR_PC_PC | 0x18; /* reset                 */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x04) = LDR_PC_PC | 0x18; /* undefined instruction */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x08) = LDR_PC_PC | 0x18; /* software interrrupt   */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x0c) = LDR_PC_PC | 0x18; /* prefetch abort        */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x10) = LDR_PC_PC | 0x18; /* data abort            */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x14) = MAGIC;            /* not used, reserved    */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x18) = LDR_PC_PC | 0x18; /* IRQ - interrupt       */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x1c) = LDR_PC_PC | 0x18; /* FIQ - interrupt       */

  /*
   * setup the secondary vector table in SRAM
   * endless loops for all exceptions but RESET and IRQ
   */
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x20) = (uint32_t) _reset;
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x24) = (uint32_t) ARM_undef;
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x28) = (uint32_t) ARM_swi;
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x2c) = (uint32_t) ARM_pAbort;
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x30) = (uint32_t) ARM_dAbort;
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x34) = 0x14;
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x38) = (uint32_t) ARM_irq;
  *(uint32_t volatile *)(AT91C_IRAM_1 + 0x3c) = (uint32_t) ARM_fiq;

  /* remap SRAM bank to address 0x0000_0000 */
  if ( MAGIC != (*(uint32_t volatile *)0x14) ) {    
    g_matrix->MATRIX_MRCR = ( AT91C_MATRIX_RCA926I |
			      AT91C_MATRIX_RCA926D );
  }

  /* configure Advanced Interrupt Controller (AIC) */
  AT91C_BASE_AIC->AIC_IDCR = ~0;       /* disable all interrupts */
  AT91C_BASE_AIC->AIC_ICCR = ~0;         /* clear all interrupts */
  for (int i = 0; i < 8; ++i) {
    AT91C_BASE_AIC->AIC_EOICR = 0;    /* write AIC_EOICR 8 times */
  }
  
  /* configure spurious ISR */
  AT91C_BASE_AIC->AIC_SPU  = (AT91_REG) irq_isr_bsp_spur;

  /* configure the PIT interrupt */
  piv = (MCK / 16 / BSP_TICKS_PER_SEC) - 1;
  AT91C_BASE_PITC->PITC_PIMR = (AT91C_PITC_PITEN | AT91C_PITC_PITIEN | piv);

  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (AT91_REG) irq_isr_bsp_pit;
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] =
    (AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED | ISR_PIT_PRIO);
  AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
  AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS);

  /* unlock IRQ/FIQ at the ARM core level */
  //ARM_INT_UNLOCK(0x1F);
}

/*****************************************************************/

uint16_t irq_get_pit_cnt(void)
{
  return g_pit_cnt;
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

void irq_isr_bsp_irq(void)
{
  typedef void (*IntVector)(void);
  IntVector vect = (IntVector)AT91C_BASE_AIC->AIC_IVR;    /* read the IVR */
                                    /* write IVR if AIC in protected mode */
  AT91C_BASE_AIC->AIC_IVR = (AT91_REG)vect;
  
  asm("MSR cpsr_c,#(0x1F)");                  /* allow nesting interrupts */
  (*vect)();          /* call the IRQ handler via the pointer to function */
  asm("MSR cpsr_c,#(0x1F | 0x80)");             /* lock IRQ before return */
  
                                    /* write AIC_EOICR to clear interrupt */
  AT91C_BASE_AIC->AIC_EOICR = (AT91_REG)vect;
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

void irq_isr_bsp_fiq(void)
{
}

/*****************************************************************/

void irq_isr_bsp_abort(char const *msg)
{
  /* this function is called when an exception occurs.
   * For production code you need to log the message and go to fail-safe
   * state. You might also want to reset the CPU.
   */

  console_putln("*** BSP abort calledå");

  for (;;) {
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

void irq_isr_bsp_spur(void)
{
}

/*****************************************************************/

void irq_isr_bsp_pit(void)
{
  console_putln("PIT ISR");
  g_pit_cnt++;
}
