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

/*
 * Bare Metal Test Irq (bmti) startup code.
 *  - https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu
 *
 *  - Building Bare-Metal ARM Systems with GNU
 *    http://www.state-machine.com
 */	

/*****************************************************************/

	/* standard definitions of Mode bits and Interrupt (I & F) flags in PSRs */
	.equ    I_BIT,          0x80      /* when I bit is set, IRQ is disabled */
	.equ    F_BIT,          0x40      /* when F bit is set, FIQ is disabled */

	.equ    USR_MODE,       0x10
	.equ    FIQ_MODE,       0x11
	.equ    IRQ_MODE,       0x12
	.equ    SVC_MODE,       0x13
	.equ    ABT_MODE,       0x17
	.equ    UND_MODE,       0x1B
	.equ    SYS_MODE,       0x1F

	/* constant to pre-fill the stack */
	.equ    STACK_FILL,     0xAAAAAAAA
	
	.text
	.code 32
	
/*****************************************************************/
	
	.global _reset
	.func	_reset

_reset:
	/* call BSP low-level initialization routine */
	LDR     r1,=_cstartup 
	MOV     lr,r1                  /* set the return address */
	LDR     sp,=__stack_end__      /* set the temporary stack pointer */
	B       bsp_low_level_init     /* call initialization */
		
_cstartup:
	/* clear the .bss section (zero init) */
	LDR     r1,=__bss_start__
	LDR     r2,=__bss_end__
	MOV     r3,#0
1:
	CMP     r1,r2
	STMLTIA r1!,{r3}
	BLT     1b

	/* fill the .stack section */
	LDR     r1,=__stack_start__
	LDR     r2,=__stack_end__
	LDR     r3,=STACK_FILL
1:
	CMP     r1,r2
	STMLTIA r1!,{r3}
	BLT     1b

	/* initialize stack pointers for all ARM modes */
	MSR     CPSR_c,#(IRQ_MODE | I_BIT | F_BIT)
	LDR     sp,=__irq_stack_top__              /* set the IRQ stack pointer */

	MSR     CPSR_c,#(FIQ_MODE | I_BIT | F_BIT)
	LDR     sp,=__fiq_stack_top__              /* set the FIQ stack pointer */

	MSR     CPSR_c,#(SVC_MODE | I_BIT | F_BIT)
	LDR     sp,=__svc_stack_top__              /* set the SVC stack pointer */

	MSR     CPSR_c,#(ABT_MODE | I_BIT | F_BIT)
	LDR     sp,=__abt_stack_top__              /* set the ABT stack pointer */

	MSR     CPSR_c,#(UND_MODE | I_BIT | F_BIT)
	LDR     sp,=__und_stack_top__              /* set the UND stack pointer */

	MSR     CPSR_c,#(SYS_MODE | I_BIT | F_BIT)
	LDR     sp,=__c_stack_top__                  /* set the C stack pointer */

	/* enter the C code */
	LDR     r12,=c_main
	MOV     lr,pc           /* set the return address */
	BX      r12             /* the target code can be ARM or THUMB */

	SWI     0xFFFFFF        /* cause exception if c_main() ever returns */

	.size   _reset, . - _reset
	.endfunc

	.end
	
