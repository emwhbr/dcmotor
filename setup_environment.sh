#!/bin/sh
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

# base directory for all tools
SAM9L9260_PATH=/opt/sam9L9260

# cross toolchain (bare metal)
CROSS_TOOLCHAIN_PATH=${SAM9L9260_PATH}/toolchains/gcc-linaro-6.3.1-2017.05-x86_64_arm-eabi
PATH=${CROSS_TOOLCHAIN_PATH}/bin:${PATH}

# OpenOCD
HOST_TOOLS_PATH=${SAM9L9260_PATH}/host_tools
PATH=${HOST_TOOLS_PATH}/bin:${PATH}

export CROSS_TOOLCHAIN_PATH
export PATH
