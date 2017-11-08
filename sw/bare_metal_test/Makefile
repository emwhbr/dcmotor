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

# ------ Directories

SRC_DIR = ./src
OBJ_DIR = ./obj

# ------ Application files

# Bare Metal Test (bmt)
APP_NAME=bmt

APP_OBJS = $(OBJ_DIR)/startup.o \
           $(OBJ_DIR)/console.o \
           $(OBJ_DIR)/led.o     \
           $(OBJ_DIR)/pwm.o     \
           $(OBJ_DIR)/adc.o     \
           $(OBJ_DIR)/encoder.o \
           $(OBJ_DIR)/main.o

# ----- Compiler flags

CFLAGS = -mcpu=$(CPU) -g -c

# ----- Assembler flags

AFLAGS = -mcpu=$(CPU) -g 

# ------ Targets

.PHONY : clean

all: app.bin

app.bin: app.elf
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(APP_NAME).elf $(OBJ_DIR)/$(APP_NAME).bin

app.elf: $(APP_OBJS)
	$(LD) -T $(SRC_DIR)/$(APP_NAME).ld $(APP_OBJS) -o $(OBJ_DIR)/$(APP_NAME).elf

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.s
	$(AS) $(AFLAGS) -o $@ $<

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	\rm -f *~
	\rm -f $(SRC_DIR)/*~
	\rm -f $(OBJ_DIR)/*.o
	\rm -f $(OBJ_DIR)/*.elf
	\rm -f $(OBJ_DIR)/*.bin
