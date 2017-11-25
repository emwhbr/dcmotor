#!/bin/bash
PRJ=`pwd`
export	GCCROOT=/usr/local/arm/gcc-4.1.2-uclibc
export	PATH=$PATH:$GCCROOT/bin
export	CROSS_COMPILE=arm-linux-uclibc-
