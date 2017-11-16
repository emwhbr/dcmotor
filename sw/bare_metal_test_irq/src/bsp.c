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
#include "startup.h"
#include "arm_exc.h"
#include "console.h"
#include "led.h"
#include "button.h"
#include "encoder.h"

/****************************************************************************
 *               Global variables
 ****************************************************************************/
static volatile uint32_t g_pit_cnt = 0;

/****************************************************************************
 *               Function prototypes
 ****************************************************************************/
static void bsp_irq_initialize(void);
static void bsp_isr_spur(void);
static void bsp_isr_pit(void);

/****************************************************************************
 *               Exported functions
 ****************************************************************************/

/*****************************************************************/

void bsp_low_level_init(void)
{
  /* Invoked by the assembler startup sequence.
   *
   * This function performs early initializations of the hardware.
   * This function cannot rely on initialization of any static variables,
   * because these have not yet been initialized in RAM.
   *
   * Most of the hardware has already been initialized by AT91Bootstrap.
   */

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
  *(uint32_t volatile *)(SRAM_ADDR + 0x00) = LDR_PC_PC | 0x18; /* reset                 */
  *(uint32_t volatile *)(SRAM_ADDR + 0x04) = LDR_PC_PC | 0x18; /* undefined instruction */
  *(uint32_t volatile *)(SRAM_ADDR + 0x08) = LDR_PC_PC | 0x18; /* software interrrupt   */
  *(uint32_t volatile *)(SRAM_ADDR + 0x0c) = LDR_PC_PC | 0x18; /* prefetch abort        */
  *(uint32_t volatile *)(SRAM_ADDR + 0x10) = LDR_PC_PC | 0x18; /* data abort            */
  *(uint32_t volatile *)(SRAM_ADDR + 0x14) = MAGIC;            /* not used, reserved    */
  *(uint32_t volatile *)(SRAM_ADDR + 0x18) = LDR_PC_PC | 0x18; /* IRQ - interrupt       */
  *(uint32_t volatile *)(SRAM_ADDR + 0x1c) = LDR_PC_PC | 0x18; /* FIQ - interrupt       */

  /*
   * setup the secondary vector table in SRAM,
   * endless loops for all exceptions but RESET
   */
  *(uint32_t volatile *)(SRAM_ADDR + 0x20) = (uint32_t) _reset;
  *(uint32_t volatile *)(SRAM_ADDR + 0x24) = (uint32_t) 0x04;
  *(uint32_t volatile *)(SRAM_ADDR + 0x28) = (uint32_t) 0x08;
  *(uint32_t volatile *)(SRAM_ADDR + 0x2c) = (uint32_t) 0x0c;
  *(uint32_t volatile *)(SRAM_ADDR + 0x30) = (uint32_t) 0x10;
  *(uint32_t volatile *)(SRAM_ADDR + 0x34) = (uint32_t) 0x14;
  *(uint32_t volatile *)(SRAM_ADDR + 0x38) = (uint32_t) 0x18;
  *(uint32_t volatile *)(SRAM_ADDR + 0x3c) = (uint32_t) 0x1c;

  /* remap first SRAM bank to address 0x0000_0000 */
  if ( MAGIC != (*(uint32_t volatile *)0x14) ) {
    AT91C_BASE_MATRIX->MATRIX_MRCR = ( AT91C_MATRIX_RCA926I |
				       AT91C_MATRIX_RCA926D );
  }
}

/*****************************************************************/

void bsp_high_level_init(void)
{
  /* Invoked early by the C main application */

  /* initialize hardware */
  console_initialize();
  led_initialize();
  button_initialize();
  encoder_initialize();

  console_putln("\nBSP init HW done");

  /* setup interrupt handling */
  bsp_irq_initialize();

  console_putln("BSP init IRQ done");
}

/*****************************************************************/

uint32_t bsp_get_pit_counter(void)
{
  uint32_t pit_cnt;
  ARM_INT_KEY_TYPE int_lock_key;  

  ARM_INT_LOCK(int_lock_key);
  pit_cnt = g_pit_cnt;
  ARM_INT_UNLOCK(int_lock_key);

  return pit_cnt;
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

void bsp_isr_irq(void)
{
  /* Invoked by ARM_irq(), arm_exc.s */

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

void bsp_isr_fiq(void)
{
  /* Invoked by ARM_fiq(), arm_exc.s */
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

void bsp_isr_abort(char const *msg)
{
  /* Invoked by ARM_except(), arm_exc.s */

  /* This function is called when an exception occurs.
   * For production code you need to log the message and go to fail-safe
   * state. You might also want to reset the CPU.
   */

  console_putln("*** BSP ISR abort");

  for (;;) {
  }
}

/****************************************************************************
 *               Private functions
 ****************************************************************************/

/*****************************************************************/

static void bsp_irq_initialize(void)
{
  uint32_t piv;
  uint32_t volatile dummy;

  /*
   * Setup the secondary vector table at address 0x0000_0000,
   * This is firt SRAM bank, remapped to 0 in bsp_low_level_init().
   */
  *(uint32_t volatile *)0x24 = (uint32_t)&ARM_undef;
  *(uint32_t volatile *)0x28 = (uint32_t)&ARM_swi;
  *(uint32_t volatile *)0x2C = (uint32_t)&ARM_pAbort;
  *(uint32_t volatile *)0x30 = (uint32_t)&ARM_dAbort;
  *(uint32_t volatile *)0x34 = (uint32_t)&ARM_reserved;
  *(uint32_t volatile *)0x38 = (uint32_t)&ARM_irq;
  *(uint32_t volatile *)0x3C = (uint32_t)&ARM_fiq;

  /* 
   * Configure Advanced Interrupt Controller (AIC)
   */
  AT91C_BASE_AIC->AIC_IDCR = ~0;    /* disable all interrupts */
  AT91C_BASE_AIC->AIC_ICCR = ~0;    /* clear all interrupts */
  for (int i = 0; i < 8; ++i) {
    AT91C_BASE_AIC->AIC_EOICR = 0;  /* write AIC_EOICR 8 times */
  }
  
  /* configure spurious ISR */
  AT91C_BASE_AIC->AIC_SPU  = (AT91_REG) bsp_isr_spur;

  /* configure the PIT interrupt */
  piv = (BSP_MCK / 16 / BSP_TICKS_PER_SEC) - 1;
  AT91C_BASE_PITC->PITC_PIMR = (AT91C_PITC_PITEN | AT91C_PITC_PITIEN | piv);

  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (AT91_REG) bsp_isr_pit;
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] =
    (AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED | ISR_PIT_PRIO);
  AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS); /* clear interrupt */
  AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS); /* enable interrupt */

  /* configure IRQ1 interrupt (user button pressed) */
  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_IRQ1] = (AT91_REG) button_isr_pressed;
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_IRQ1] =
    (AT91C_AIC_SRCTYPE_EXT_POSITIVE_EDGE | ISR_BUT_PRIO);
  AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_IRQ1); /* clear interrupt */
  AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_IRQ1); /* enable interrupt */

  /* configure PIOB interrupt (encoder outputs A & B) */
  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_PIOB] = (AT91_REG) encoder_isr_output_ab;
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_PIOB] =
    (AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED | ISR_ENC_PRIO);
  AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_PIOB); /* clear interrupt */
  AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_PIOB); /* enable interrupt */

  dummy = AT91C_BASE_PIOB->PIO_ISR; /* clear any pending interrupts */
  AT91C_BASE_PIOB->PIO_IER = ENCODER_PIN_ENCA; /* enable interrupt encoder A */
  AT91C_BASE_PIOB->PIO_IER = ENCODER_PIN_ENCB; /* enable interrupt encoder B */

  /* unlock IRQ/FIQ at the ARM core level */
  ARM_INT_UNLOCK(0x1F);
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

static void bsp_isr_spur(void)
{
  /* 
   * BSP ISR: Spurious interrupts
   */
  console_putln("BSP ISR : spur");
}

/*****************************************************************/

__attribute__ ((section (".text.fastcode")))

static void bsp_isr_pit(void)
{
  /* 
   * BSP ISR: Programmable Interval Timer (PIT)
   */

  ARM_INT_KEY_TYPE int_lock_key;

  /* clear this interrupt */
  uint32_t volatile dummy = AT91C_BASE_PITC->PITC_PIVR;

  ARM_INT_LOCK(int_lock_key);
  g_pit_cnt++;
  ARM_INT_UNLOCK(int_lock_key);
}
