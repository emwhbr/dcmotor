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

include ./common_defs.mk

# ------ Targets

all: force_output build

force_output:
	@echo "COMPILER : $(CC) $(CFLAGS)"
	@echo "ASSEMBLER: $(AS) $(AFLAGS)"

build:
	@make all --no-print-directory -C bsp
	@make all --no-print-directory -C proc
	@make all --no-print-directory -C core

clean:
	@make clean --no-print-directory -C bsp
	@make clean --no-print-directory -C proc
	@make clean --no-print-directory -C core
	@\rm -f *~

help:
	@echo "Usage: make clean   Clean build"
	@echo "       make all     Build application"
