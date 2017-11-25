-----------------------------------
----      FILE DESCRIPTION     ----
-----------------------------------

File : at91bootstrap-2.4-olimex.tar.bz2
---------------------------------------
Source code for the AT91Bootstrap (Olimex version).
Downloaded from TBD.


File: at91bootstrap-2.4-olimex-hbr_v1.patch
-------------------------------------------
This patch file was the first try when generating the AT91Bootstrap using
my own toolchain. Added a new board (sam9_l9260_hbr).


File: at91bootstrap-2.4-olimex-hbr_v2.patch
-------------------------------------------
This patch file was the second try. Added some debug printouts and
added the generation of list-files in the Makefile. The generated binary
is DataFlash version only and was successfully executed using the hw-debugger
and OCD. It was possible to start original U-Boot in DataFlash. Also tested
using Eclipse and debugging with hw-debugger and OCD.

This patch assumes nothing and shall be applied directly on the original
source code.


File: at91bootstrap-2.4-olimex-hbr_v3.patch
-------------------------------------------
This patch file adds the NandFlash version. Both binaries was successfully
executed using the hw-debugger and OCD. It was possible to start my own
U-Boot from NandFlash. Also tested to program the NandFlash version to the
NandFlash, Removing jumper DF_E forced RomBoot to start the bootstrap!!!
Removed lots of dead code and streamlined the printouts to make the binaries
smaller then the absolute maximum of 4096 bytes.

sam9_l9260_hbr-dataflashboot-2.4.bin (3650 bytes)
sam9_l9260_hbr-nandflashboot-2.4.bin (4080 bytes)

This patch assumes nothing and shall be applied directly on the original
source code.


File: at91bootstrap-2.4-olimex-hbr_v4.patch
-------------------------------------------
This patch file is almost the same as the prevoius (v3).
The size of the NandFlash version has been reduced by removing definitions
for Flash devices that are not used on this board. No changes to the DataFlash
version. Both binaries was successfully executed using the hw-debugger and OCD.

sam9_l9260_hbr-dataflashboot-2.4.bin (3650 bytes)
sam9_l9260_hbr-nandflashboot-2.4.bin (3952 bytes)

This patch assumes nothing and shall be applied directly on the original
source code.


File: at91bootstrap-2.4-olimex-hbr_v5.patch
-------------------------------------------
This patch file is almost the same as the prevoius (v4).
Changed the start address of U-Boot in NandFlash.
Old address: 0x1D20_0000
New address: 0x1CF0_0000

sam9_l9260_hbr-dataflashboot-2.4.bin (3650 bytes)
sam9_l9260_hbr-nandflashboot-2.4.bin (3952 bytes)

Initial breakpoints when bypassing load of U-Boot:
dataflash: 0x200260
nandflash: 0x2002BC

This patch assumes nothing and shall be applied directly on the original
source code.


File: at91bootstrap-2.4-olimex-hbr_v6.patch
-------------------------------------------
Built on the prevoius patch (v5).

* Changed size of U-Boot in NAND, from 0xE00000 to 0xE0000 bytes.
  The size was prevoius to large, now using real size: 896K.
  To much data was copied to SDRAM, now less data is copied, faster load time.

* Now using the LED's to signal status:
  On init:  STAT (green) is on, PWR_LED (yellow) is off.
  On error: STAT (green) is off, PWR_LED (yellow) is on.

* Commented out unused code in 'driver/gpio.c' to save space.

sam9_l9260_hbr-dataflashboot-2.4.bin (3612 bytes)
sam9_l9260_hbr-nandflashboot-2.4.bin (3908 bytes)

Initial breakpoints when bypassing load of U-Boot:
dataflash: 0x20026C
nandflash: 0x2002C8

This patch assumes nothing and shall be applied directly on the original
source code.


File: at91bootstrap-2.4-olimex-hbr_v6_gcc441.patch
--------------------------------------------------
Same functionality as in 'at91bootstrap-2.4-olimex-hbr_v6.patch'.
Changes to make it compatible with new cross-toolchain.

* Compiler flag -g removed to make resulting binary fit in SRAM.
  No debug information available.

sam9_l9260_hbr-dataflashboot-2.4.bin (3616 bytes)
sam9_l9260_hbr-nandflashboot-2.4.bin (3896 bytes)

Initial breakpoints when bypassing load of U-Boot:
dataflash: ?
nandflash: 0x200300


File: at91bootstrap-2.4-olimex-hbr_v7_gcc441.patch
--------------------------------------------------
Built on the prevoius patch (v6_gcc441).

* New CPU frequency and SDRAM settings ("fastboot").
  CPU (new): 204.800 MHz     Master clock (new): 102.400 MHz
  CPU (old): 200.704 MHz     Master clock (old):  50.176 MHz

* Rewritten initialization code for SDRAM controller.
  Code is now in assembler and sequence is according to data sheet.

* Minimized size of data copied when U-Boot is read from NandFlash
  to SDRAM. New size is 256 KB (old size was 896 KB).

* Dataflash version is not longer maintained.

sam9_l9260_hbr-nandflashboot-2.4.bin (3768 bytes)

Initial breakpoint when bypassing load of U-Boot:
nandflash: 0x200300

This patch assumes nothing and shall be applied directly on the original
source code.


File: at91bootstrap-2.4-olimex-hbr_v8_dualboot_gcc441.patch
------------------------------------------------------------
Built on the prevoius patch (v7_gcc441).

* New timing values for NAND Flash.

* Added "dual-boot" using button for booting
  a) U-Boot
  b) Kernel directly (bypass U-Boot)

* Redefinitions of data types to  shrink size of the final bin-file.

* Reintroduced compiler flag -g in Makefile.
  Debug information is now available again.

sam9_l9260_hbr-nandflashboot-2.4.bin (3944 bytes)

Initial breakpoint when bypassing load of U-Boot:
nandflash: 0x200344

This patch assumes nothing and shall be applied directly on the original
source code.


--------------------------------------------------------
----   HOW TO APPLY PATCHES AND BUILD BOOTSTRAP     ----
--------------------------------------------------------

1. Unpack bootstrap source code
-------------------------------
tar xvf at91bootstrap-2.4-olimex.tar.bz2
cd at91bootstrap-2.4-olimex

2. Apply HBR-patch
--------------------
patch -Np1 < ../at91bootstrap-2.4-olimex-hbr_v7_gcc441.patch

3. Build NandFlash version
--------------------------
make CROSS_COMPILE=arm-unknown-linux-gnueabi- clean
make CROSS_COMPILE=arm-unknown-linux-gnueabi- sam9_l9260_hbr_defconfig_nf
make CROSS_COMPILE=arm-unknown-linux-gnueabi-

This will generate 'sam9_l9260_hbr-nandflashboot-2.4.bin' (3944 bytes).

4. How to disassemble (for getting breakpoint address)
------------------------------------------------------
arm-unknown-linux-gnueabi-objdump -S \
result/sam9_l9260_hbr-nandflashboot-2.4.elf > \
result/sam9_l9260_hbr-nandflashboot-2.4.dis

Examine disassembled file.
Look for two 'nop' instructions in function 'hw_init'.

E.g)
/* Dummy code, for setting breakpoint with OpenOCD */
	__asm__("nop");
200344:	e1a00000 	nop			(mov r0,r0)
	__asm__("nop");
200348:	e1a00000 	nop			(mov r0,r0)

Breakpoint address is: 0x200344
