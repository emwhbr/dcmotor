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

# ------ Files

OBJS = $(OBJ_DIR)/arm_exc.o \
       $(OBJ_DIR)/startup.o

# ------ Targets

all: $(OBJS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.s
	@$(AS) $(AFLAGS) -o $@ $<
	@echo "[AS] $<"

clean:
	@\rm -f *~
	@\rm -f $(SRC_DIR)/*~
	@\rm -f $(OBJ_DIR)/*.o
