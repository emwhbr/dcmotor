# /************************************************************************
#  *                                                                      *
#  * Copyright (C) 2017 Bonden i Nol (hakanbrolin@hotmail.com)            *
#  *                                                                      *
#  * This program is free software; you can redistribute it and/or modify *
#  * it under the terms of the GNU General Public License as published by *
#  * the Free Software Foundation; either version 2 of the License, or    *
#  * (at your option) any later version.                                  *
#  *                                                                      *
#  ************************************************************************/

# ----- Toolchain setup
CROSS_COMPILE = arm-eabi-

CPU=arm926ej-s

AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CC) -E
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# ----- Compiler flags

CFLAGS = -mcpu=$(CPU) -mfloat-abi=soft -g -c

# ----- Assembler flags

AFLAGS = -mcpu=$(CPU) -mfloat-abi=soft -g

# ----- Linker flags

LD_PATH = ${CROSS_TOOLCHAIN_PATH}/lib/gcc/arm-eabi/6.3.1

LFLAGS = -L$(LD_PATH) 
LFLAGS += -lgcc
