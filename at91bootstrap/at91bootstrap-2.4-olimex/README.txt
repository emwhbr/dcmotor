===========================================================================================
DataflashBoot for AT91SAM926x.

******* WARNING ***************
*
*	This version of dataflashboot loads data from 0x8400.
*
***********************************



1 Compiling an at91bootstrap project

1.1 GNU ARM Toolchain
===========================================================================================
1.1.1 Linux

AT91Bootstrap has been compiled and tested under Linux using the following a gcc
built by buildroot. You can build a buildroot toolchain by doing

$ make toolchain

This will download buildroot from "buildroot.uclibc.org".
You will have to configure buildroot to build a cross compiler:
AT91-Bootstrap has been tested with gcc-3.4.6 and binutils-2.16.1, uclibc-0.9.28

After building a toolchain, you should set it up according to the example: gcc-3.4.6-uclibc.sh

1.1.2 Windows

Thjis has not been tested by this version, download the vanilla at91-bootstrap and
build according to instructions.

1.2 Compiling DataFlashBoot-2.1
===========================================================================================

1.2.1 AT91SAM9260-EK
---------------------
$ make at91sam9260ek_dataflash

Configurations are in 
* board/at91sam9260ek/at91sam9260ek.mk
* board/at91sam9260ek/dataflash/at91sam9260ek.h
and can be modified according your needs.

1.2.2 AT91SAM9261-EK
---------------------
$ make at91sam9261ek_dataflash

Configurations are in 
* board/at91sam9260ek/at91sam9261ek.mk
* board/at91sam9260ek/dataflash/at91sam9261ek.h
and can be modified according your needs.

1.2.3 AT91SAM9263-EK
---------------------
$ make at91sam9263ek_dataflash

Configurations are in 
* board/at91sam9260ek/at91sam9260ek.mk
* board/at91sam9260ek/dataflash/at91sam9263ek.h
and can be modified according your needs.


1.3 Compiling NANDFlashBoot
===========================================================================================
1.3.1 AT91SAM9260-EK
---------------------
$ make at91sam9260ek_nandflash

Configurations are in 
* board/at91sam9260ek/at91sam9260ek.mk
* board/at91sam9260ek/nandflash/at91sam9260ek.h
and can be modified according your needs.

1.3.2 AT91SAM9261-EK
---------------------
$ make at91sam9261ek_nandflash

Configurations are in 
* board/at91sam9261ek/at91sam9261ek.mk
* board/at91sam9261ek/nandflash/at91sam9261ek.h
and can be modified according your needs.

1.3.3 AT91SAM9263-EK
---------------------
$ make at91sam9263ek_nandflash

Configurations are in 
* board/at91sam9263ek/at91sam9260ek.mk
* board/at91sam9263ek/nandflash/at91sam9263ek.h
and can be modified according your needs.


===================================================================================================
===  WARNING for AT91SAM9260 Data/NANDFlashBoot							===
===================================================================================================
DataFlashBoot binary image size must be less than 4kBytes as it is the AT91SAM9260 internal available SRAM size.

According the GCC toolchain which is used (GCC-3.4 Toolchain or less), resulting code size
may be higher than the allowed 4kBytes. In such a case, either update your GCC toolchain to
a more recent one (GCC-4.0 Toolchain or higher) or do not use the provided gpio driver to
configure SDRAM PIOs for example. Indeed, replace sdramc_hw_init() function in
board/at91sam9260ek.c source file by:

#ifdef CFG_SDRAM
void sdramc_hw_init(void)
{
/* Configure the PIO controller to enable 32-bits SDRAM */
writel(0xFFFF0000, AT91C_BASE_PIOC + PIO_ASR(0));
writel(0xFFFF0000, AT91C_BASE_PIOC + PIO_PDR(0));
}
#endif

Note: Code is less readable but it should be sufficient enough to have less than 4kBytes code size
without having to re-compile a complete GCC toolchain.
