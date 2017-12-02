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

include ../common_defs.mk

# ------ Directories

SRC_DIR = .
OBJ_DIR = ./obj

PROC_DIR = ../proc

# ------ Files

OBJS = $(OBJ_DIR)/adc.o      \
       $(OBJ_DIR)/bsp.o      \
       $(OBJ_DIR)/button.o   \
       $(OBJ_DIR)/console.o  \
       $(OBJ_DIR)/dbg.o      \
       $(OBJ_DIR)/encoder.o  \
       $(OBJ_DIR)/led.o      \
       $(OBJ_DIR)/motor.o    \
       $(OBJ_DIR)/pwm.o

# ----- Compiler includes

PROC_INCLUDE = -I$(PROC_DIR)

INCLUDES = $(PROC_INCLUDE)

# ------ Targets

all: $(OBJS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
	@echo "[CC] $<"

clean:
	@\rm -f *~
	@\rm -f $(SRC_DIR)/*~
	@\rm -f $(OBJ_DIR)/*.o
