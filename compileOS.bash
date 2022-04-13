#!/bin/bash
source ../PathSetter.bash
#compile the kernel file
bcc -ansi -c -o kernel.o kernel.c
#assemble the kernel.asm file
as86 kernel.asm -o kernel_asm.o
#link the kernel.o and kernel.asm files into the executable kernel file
ld86 -o kernel -d kernel.o kernel_asm.o
#copy the kernel file into the disk image at sector 3 where the bootloader is expecting to find it
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
#run bochs to allow the bootloader to load the kernel 
bochs -f opsys.bxrc
