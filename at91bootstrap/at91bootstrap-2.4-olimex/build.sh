#!/bin/bash
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

# toolchain setup
TC_PREFIX=arm-eabi-

################################################################
function print_usage_and_die()
################################################################
{
    echo "Usage: $0 <rel|clean>"
    echo ""
    echo "rel    Build release"
    echo "clean  Build clean"
    exit 1  
}

################################################################
function do_build_rel()
################################################################
{
    echo "==[BUILD RELEASE]==="

    # build NandFlash version
    make CROSS_COMPILE=${TC_PREFIX} sam9_l9260_hbr_defconfig_nf
    make CROSS_COMPILE=${TC_PREFIX} all

    # disassemble
    ${TC_PREFIX}objdump -S \
	result/sam9_l9260_hbr-nandflashboot-2.4.elf > \
	result/sam9_l9260_hbr-nandflashboot-2.4.dis

    exit $?
}

################################################################
function do_build_clean()
################################################################
{
    echo "==[BUILD CLEAN]==="

    make CROSS_COMPILE=${TC_PREFIX} clean

    exit $?
}

#######################################################################
##############                MAIN                         ############
#######################################################################
# check environment
which ${TC_PREFIX}gcc > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "*** ERROR: toolchain (${TC_PREFIX}gcc) not found, setup environment!"
    exit 1
fi

# do the build
case "$1" in
    rel)
	do_build_rel
        ;;

    clean)
	do_build_clean
        ;;

    *)
        print_usage_and_die
        ;;
esac
