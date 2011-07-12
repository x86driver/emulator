TARGET = emulator

CFLAGS = -g

all:$(TARGET)

aluop.o:aluop.c
	gcc -Wall -c -o $@ $< $(CFLAGS)

emulator:aluop.o main.c inst.h
	gcc -Wall -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o
