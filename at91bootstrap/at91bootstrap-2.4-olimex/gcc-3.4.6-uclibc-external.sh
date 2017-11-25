#!/bin/bash
PRJ=`pwd`
export	GCCROOT=/usr/local/arm/gcc-3.4.6-uclibc
export	PATH=$PATH:$GCCROOT/bin
export	CROSS_COMPILE=arm-linux-
