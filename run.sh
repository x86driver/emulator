#!/bin/sh
arm-none-linux-gnueabi-as -o a.o a.s
arm-none-linux-gnueabi-objcopy -I elf32-littlearm -O binary a.o a.bin

