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

BSP_DIR  = ../bsp
PROC_DIR = ../proc

# ------ Files

OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/mpc_fsm.o \
       $(OBJ_DIR)/util.o

# ----- Application

# Motor Position Control (mpc)
APP_NAME = mpc
APP_ELF  = $(OBJ_DIR)/$(APP_NAME).elf
APP_DIS  = $(OBJ_DIR)/$(APP_NAME).dis
APP_BIN  = $(OBJ_DIR)/$(APP_NAME).bin

# ----- Compiler includes

BSP_INCLUDE  = -I$(BSP_DIR)
PROC_INCLUDE = -I$(PROC_DIR)

INCLUDES = $(BSP_INCLUDE) $(PROC_INCLUDE)

# ----- Linker directives

LD_DIRS = -L$(BSP_DIR)/obj -L$(PROC_DIR)/obj

LD_SCRIPT = $(SRC_DIR)/$(APP_NAME).ld

BSP_OBJS  = $(wildcard $(BSP_DIR)/obj/*.o)
PROC_OBJS = $(wildcard $(PROC_DIR)/obj/*.o)
ALL_OBJS  = $(BSP_OBJS) $(PROC_OBJS) $(OBJS)
#ALL_OBJS  = $(PROC_OBJS) $(BSP_OBJS) $(OBJS)

# ------ Targets

all: app.bin

app.bin: app.elf
	@$(OBJCOPY) -O binary $(APP_ELF) $(APP_BIN)
	@echo "[BIN] $(APP_BIN)"

app.elf: $(OBJS)	
	@$(LD) -T $(LD_SCRIPT) $(LD_DIRS) $(ALL_OBJS) $(LFLAGS) -o $(APP_ELF)
	@echo "[ELF] $(APP_ELF)"	
	@$(OBJDUMP) -d -S $(APP_ELF) > $(OBJ_DIR)/$(APP_NAME).dis
	@echo "[DIS] $(APP_DIS)"

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
	@echo "[CC] $<"

clean:
	@\rm -f *~
	@\rm -f $(SRC_DIR)/*~
	@\rm -f $(OBJ_DIR)/*.o
	@\rm -f $(OBJ_DIR)/*.elf
	@\rm -f $(OBJ_DIR)/*.dis
	@\rm -f $(OBJ_DIR)/*.bin

