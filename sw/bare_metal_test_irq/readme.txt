Disassemble
-----------
arm-eabi-objdump -d -D    obj/bmti.elf | less
arm-eabi-objdump -d -D -S obj/bmti.elf | less

ELF information
---------------
arm-eabi-readelf -l obj/bmti.elf
arm-eabi-readelf -S obj/bmti.elf

