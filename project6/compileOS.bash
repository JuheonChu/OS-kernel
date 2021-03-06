#!/bin/bash
#refresh the path setting
source ../PathSetter.bash

# create a disk image filled with 0's
dd if=/dev/zero of=floppya.img bs=512 count=2880

# assemble to install the bootloader
nasm bootload.asm

#copy the boatload program to sector 0 of the floppya.img disk image
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc seek=0 

#set up initial file system to store the Disk Map
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

#compile all the c files below.
bcc -ansi -c -o kernel.o kernel.c
bcc -ansi -c -o uprog1.o uprog1.c
bcc -ansi -c -o uprog2.o uprog2.c
bcc -ansi -c -o uprog3.o uprog3.c
bcc -ansi -c -o uprog4.o uprog4.c
bcc -ansi -c -o shell.o shell.c
bcc -ansi -c -o userLib.o userLib.c
bcc -ansi -c -o textEd.o textEd.c
bcc -ansi -c -o proc.o proc.c
bcc -ansi -c –o uprog6.o uprog6.c
bcc -ansi -c –o uprog7.o uprog7.c
bcc -ansi -c -o who.o who.c 


#assemble kernel.asm & lib.asm file
as86 kernel.asm -o kernel_asm.o
as86 lib.asm -o lib.o

#link the object files and place the resulting executable file on the disk
ld86 -o kernel -d kernel.o proc.o kernel_asm.o 
ld86 -o uprog1 -d uprog1.o lib.o
ld86 -o uprog2 -d uprog2.o lib.o 
ld86 -o uprog3 -d uprog3.o userLib.o lib.o
ld86 -o uprog4 -d uprog4.o userLib.o lib.o
ld86 -o shell -d shell.o userLib.o lib.o
ld86 -o textEd -d textEd.o userLib.o lib.o
ld86 -o uprog6 -d uprog6.o lib.o userlib.o
ld86 -o uprog7 -d uprog7.o lib.o userlib.o
ld86 -o who -d who.o lib.o userlib.o 

#copy the kernel file into the disk image at sector 3 
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3



#place the provided message.txt file into sector 30 of the disk after copying in the kernel.
dd if=message.txt of=floppya.img bs=512 conv=notrunc seek=30

# compile with gcc compiler for loadFile.c
gcc -o loadFile loadFile.c


# copy the contents of the executable files to disk image
./loadFile message.txt
./loadFile uprog1
./loadFile uprog2
./loadFile uprog3
./loadFile uprog4
./loadFile shell
./loadFile textEd
./loadfile uprog6
./loadfile uprog7
./loadfile who


#run bochs to allow the bootloader to load the kernel 
bochs -f opsys.bxrc
