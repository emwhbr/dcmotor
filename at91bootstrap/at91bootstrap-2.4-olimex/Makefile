AS=$(CROSS_COMPILE)gcc
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
NM= $(CROSS_COMPILE)nm
SIZE=$(CROSS_COMPILE)size
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump

TOPDIR	:=	$(CURDIR)

VERSION:=2.4

include	.config

ifeq	($(CONFIG_SPI_MODE),)
CONFIG_SPI_MODE=SPI_MODE_0
endif

ifeq	($(CONFIG_SPI_CLK),)
CONFIG_SPI_CLK=33000000
endif

ifndef	MEMORY
MEMORY:=dataflash
endif

ifeq ($(MEMORY),dataflash)
CFG_MEMORY=DATAFLASH
endif
ifeq ($(MEMORY),dataflashcard)
CFG_MEMORY=DATAFLASHCARD
endif
ifeq ($(MEMORY),nandflash)
CFG_MEMORY=NANDFLASH
endif
ifeq ($(MEMORY),flash)
CFG_MEMORY=FLASH
endif



COBJS-y:= main.o board/$(BOARD)/$(BOARD).o
SOBJS-y:= crt0_gnu.o

ifdef	BOARD
include	driver/*.mk
include	board/$(BOARD)/$(BOARD).mk
else

endif

ifndef BOOT_NAME
BOOT_NAME=$(BOARDNAME)-$(PROJECT)boot-$(VERSION)
endif


# $(SOBJS-y:.o=.S) 
SRCS	:= $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS-y) $(COBJS-y))

INCL=board/$(BOARD)/$(PROJECT)

CPPFLAGS=-g -mcpu=arm9 -Os -Wall -D$(TARGET)		\
	-I$(INCL) -Iinclude				\
	-DTOP_OF_MEM=$(TOP_OF_MEMORY) 			\
	-D$(SPI_MODE) -D$(SPI_BOOT) -DSPI_CLK=$(SPI_CLK) \
	$(AT91_CUSTOM_FLAGS)

ASFLAGS=-g -mcpu=arm9 -Os -Wall -D$(TARGET) -I$(INCL) -Iinclude -DTOP_OF_MEM=$(TOP_OF_MEMORY)

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to map file
#  -lc 	   : 	tells the linker to tie in newlib
#  -lgcc   : 	tells the linker to tie in newlib
LDFLAGS+=-nostartfiles -Map=result/$(BOOT_NAME).map --cref
#LDFLAGS+=-lc -lgcc
LDFLAGS+=-T elf32-littlearm.lds -Ttext $(LINK_ADDR) --relax

all: 	binaries/$(BOOT_NAME).bin

rebuild: clean all

.SUFFIXES:	.c

result/$(BOOT_NAME).elf: $(OBJS)
	mkdir -p result
	echo $(CONFIG_DATAFLASH) $(COBJS-y)
	$(LD) $(LDFLAGS) -n -o result/$(BOOT_NAME).elf $(OBJS)

binaries/$(BOOT_NAME).bin: result/$(BOOT_NAME).elf
	mkdir -p binaries
	$(OBJCOPY) --strip-debug --strip-unneeded result/$(BOOT_NAME).elf -O binary binaries/$(BOOT_NAME).bin

crt0_gnu.o:  crt0_gnu.S
	$(AS)  $(ASFLAGS)  -c -o crt0_gnu.o crt0_gnu.S 

at91sam9260dfc_defconfig:	clean
	echo	BOARDNAME=at91sam9260dfc	>	.config
	echo	BOARD=at91sam9260ek		>>	.config
	echo	PROJECT=dataflashcard		>>	.config
	echo	CONFIG_DATAFLASH=y		>>	.config
	echo	SPI_MODE=$(CONFIG_SPI_MODE)	>>	.config
	echo	SPI_BOOT=SPI_BOOT_CS0		>>	.config
	echo	SPI_CLK=$(CONFIG_SPI_CLK)	>>	.config

at91sam9260ek_defconfig:	clean
	echo	BOARDNAME=at91sam9260ek		>	.config
	echo	BOARD=at91sam9260ek		>>	.config
	echo	PROJECT=$(MEMORY)		>>	.config
	echo	CONFIG_$(CFG_MEMORY)=y		>>	.config
	echo	SPI_MODE=$(CONFIG_SPI_MODE)	>>	.config
	echo	SPI_BOOT=SPI_BOOT_CS1		>>	.config
	echo	SPI_CLK=$(CONFIG_SPI_CLK)	>>	.config

sam9_l9260_defconfig:	clean
	echo	BOARDNAME=sam9_l9260		>	.config
	echo	BOARD=sam9_l9260		>>	.config
	echo	PROJECT=$(MEMORY)		>>	.config
	echo	CONFIG_$(CFG_MEMORY)=y		>>	.config
	echo	SPI_MODE=$(CONFIG_SPI_MODE)	>>	.config
	echo	SPI_BOOT=SPI_BOOT_CS1		>>	.config
	echo	SPI_CLK=$(CONFIG_SPI_CLK)	>>	.config

at91sam9260pf_defconfig:	clean
	echo	BOARDNAME=at91sam9260pf		>	.config
	echo	BOARD=at91sam9260ek		>>	.config
	echo	PROJECT=$(MEMORY)		>>	.config
	echo	CONFIG_$(CFG_MEMORY)=y		>>	.config
	echo	SPI_MODE=$(CONFIG_SPI_MODE)	>>	.config
	echo	SPI_BOOT=SPI_BOOT_CS0		>>	.config
	echo	SPI_CLK=$(CONFIG_SPI_CLK)	>>	.config

at91sam9261ek_defconfig:	clean
	echo	BOARDNAME=at91sam9261ek		>	.config
	echo	BOARD=at91sam9261ek		>>	.config
	echo	PROJECT=$(MEMORY)		>>	.config
	echo	CONFIG_$(CFG_MEMORY)=y		>>	.config
	echo	SPI_MODE=$(CONFIG_SPI_MODE)	>>	.config
	echo	SPI_BOOT=SPI_BOOT_CS0		>>	.config
	echo	SPI_CLK=$(CONFIG_SPI_CLK)	>>	.config

sam9_l9261_defconfig:	clean
	echo	BOARDNAME=sam9_l9261		>	.config
	echo	BOARD=sam9_l9261		>>	.config
	echo	PROJECT=$(MEMORY)		>>	.config
	echo	CONFIG_$(CFG_MEMORY)=y		>>	.config
	echo	SPI_MODE=$(CONFIG_SPI_MODE)	>>	.config
	echo	SPI_BOOT=SPI_BOOT_CS0		>>	.config
	echo	SPI_CLK=$(CONFIG_SPI_CLK)	>>	.config

at91sam9263ek_defconfig:	clean
	echo	BOARDNAME=at91sam9263ek		>	.config
	echo	BOARD=at91sam9263ek		>>	.config
	echo	PROJECT=$(MEMORY)		>>	.config
	echo	CONFIG_$(CFG_MEMORY)=y		>>	.config
	echo	SPI_MODE=$(CONFIG_SPI_MODE)	>>	.config
	echo	SPI_BOOT=SPI_BOOT_CS0		>>	.config
	echo	SPI_CLK=$(CONFIG_SPI_CLK)	>>	.config

../buildroot:
	(cd .. ; svn co svn://uclibc.org/trunk/buildroot)

../buildroot/.config:	../buildroot
	cp buildroot.config $@
	make -C ../buildroot menuconfig

toolchain:	../buildroot/.config
	make -C ../buildroot

#	echo $PCK 1  $(PCK) 2  ${PCK} 3 $$PCK 4  $$(PCK) 5  $${PCK}  )
	
#	cd .. ; tar -cvf `basename $$(TOPDIR)`.tar `basename $$(TOPDIR)` ; bzip2 `basename $$(TOPDIR)`.tar )

distrib:
	find . -type f \( -name .depend \
		-o -name '*.srec' \
		-o -name '*.bin' \
		-o -name '*.elf' \
		-o -name '*.map' \
		-o -name '*.o' \
		-o -name '*~' \) \
		-print0 \
		| xargs -0 rm -f
	rm -fr binaries
	rm -fr result

clean:
	find . -type f \( -name .depend \
		-o -name '*.srec' \
		-o -name '*.elf' \
		-o -name '*.map' \
		-o -name '*.o' \
		-o -name '*~' \) \
		-print0 \
		| xargs -0 rm -f
	rm -fr result

tarball:	clean
	F=`basename $(TOPDIR)` ; cd .. ; \
	tar --force-local -cvf $$F.tar $$F; \
	rm -f $$F.tar.bz2 ; \
	bzip2 $$F.tar ; \
	cp -f $$F.tar.bz2 /usr/local/install/downloads
