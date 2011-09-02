TARGET = hello

all:$(TARGET)

hello:hello.c
	arm-none-linux-gnueabi-gcc -static -o hello hello.c -nostdlib -e main

clean:
	rm -rf $(TARGET)