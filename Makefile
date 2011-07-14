TARGET = emulator
OBJ = aluop.o reg.o mem.o load_store.o function.o

CFLAGS = -g

all:$(TARGET)

function.o:function.c function.h env.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

aluop.o:aluop.c inst.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

reg.o:reg.c reg.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

mem.o:mem.c mem.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

load_store.o:load_store.c load_store.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

emulator:$(OBJ) main.c inst.h env.h
	gcc -Wall -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o
