TARGET = libdisas-arm.a

all:$(TARGET)

arm-dis.o:arm-dis.c
	gcc -Wall -c -o $@ $< $(CFLAGS)

disas.o:disas.c
	gcc -Wall -c -o $@ $< $(CFLAGS)

libdisas-arm.a:arm-dis.o disas.o
	ar rcs $@ $^

clean:
	rm -rf $(TARGET) *.o
