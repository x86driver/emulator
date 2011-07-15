#!/bin/sh
./run.sh
arm-none-linux-gnueabi-objdump -d a.o > a.txt
sed -i -e '1,7d' a.txt
./emulator > b.txt
diff a.txt b.txt

