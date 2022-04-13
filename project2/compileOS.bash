#!/bin/bash
#refresh the path setting
source ../PathSetter.bash

#compile the kernel.c
bcc -ansi -c -o kernel.o kernel.c

#assemble kernel.asm
as86 kernel.asm -o kernel_asm.o

#link the kernel.o and kernel.asm files 
ld86 -o kernel -d kernel.o kernel_asm.o

#copy the kernel file into the disk image at sector 3 
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

#place the provided message.txt file into sector 30 of the disk after copying in the kernel.
dd if=message.txt of=floppya.img bs=512 conv=notrunc seek=30

#run bochs to allow the bootloader to load the kernel 
bochs -f opsys.bxrc
