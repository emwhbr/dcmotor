# Makefile for AT91-Bootstrap drivers directory

COBJS-y				+=	driver/debug.o
COBJS-y				+=	driver/gpio.o
COBJS-y				+=	driver/pmc.o
COBJS-y				+=	driver/sdramc.o
COBJS-y				+=	driver/string.o
COBJS-y				+=	driver/udiv.o
COBJS-$(CONFIG_NANDFLASH)	+=	driver/nandflash.o
COBJS-$(CONFIG_DATAFLASH)	+=	driver/dataflash.o
COBJS-$(CONFIG_FLASH)		+=	driver/flash.o
