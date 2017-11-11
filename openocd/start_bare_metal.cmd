##
## Filename:
## start_bare_metal.cmd
##
## Description:
## Command sequence for running a bare metal test application.
## The Bootstrap(NandFlash version) performs necessary HW-initialization.
## Bypassing the load of U-Boot from NandFlash.
## Instead, the test application is loaded to directly to SDRAM from file.
## 
## Usage:
## 1) Start OpenOCD
##    > openocd --file ./openocd.cfg
##
## 2) This file should be executed from a Telnet session:
##    > telnet localhost 1973
##    > source start_bare_metal.cmd
##

# Reset and halt target, remove old breakpoint
echo ">>>> Reset and halt"
init
reset halt
reset init
rbp 0x200344
halt

# Enable fast memory access
echo ">>>> Enable fast memory"
arm7_9 fast_memory_access enable

# Set JTAG speed to 1MHz
echo ">>>> Set JTAG speed to 1MHz"
jtag_khz 1000

# Load the AT91Bootstrap image
echo ">>>> Load bootstrap bin file to SRAM"
load_image ../fw/sam9_l9260_hbr-nandflashboot-2.4.bin 0x200000 bin

# Excecute AT91Bootstrap, stop before loading U-Boot from NandFlash
echo ">>>> Starting bootstrap with breakpoint"
bp 0x200344 4
resume 0x200000

######## Run the commands below manually - Doesn't work in script mode 
# Load bare metal test application image to SDRAM
echo ">>>> Start bare metal test application"
#load_image ../sw/bare_metal_test/obj/bmt.bin 0x22200000 bin
#load_image ../sw/bare_metal_test_irq/obj/bmti.bin 0x22200000 bin
#resume 0x22200000

