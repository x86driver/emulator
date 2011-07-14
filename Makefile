TARGET = emulator
OBJ = aluop.o reg.o mem.o load_store.o function.o cond.o

CFLAGS = -g

all:$(TARGET)

cond.o:cond.c cond.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

function.o:function.c function.h env.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

aluop.o:aluop.c inst.h env.h cond.h aluop.h reg.h load_store.h utils.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

reg.o:reg.c reg.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

mem.o:mem.c mem.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

load_store.o:load_store.c inst.h env.h cond.h aluop.h mem.h reg.h utils.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

emulator:$(OBJ) main.c inst.h env.h
	gcc -Wall -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o
