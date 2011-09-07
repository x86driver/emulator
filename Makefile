TARGET = emulator
OBJ = aluop.o reg.o mem.o load_store.o function.o cond.o branch.o env.o super.o misc.o

LIBDIR = disas-arm
LIBS = disas-arm

CFLAGS = -g -Wextra

all:$(TARGET)

disas-arm.a:
	cd $(PWD)/disas-arm; make

misc.o: misc.c inst.h env.h cond.h aluop.h reg.h misc.h branch.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

super.o:super.c env.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

env.o: env.c env.h reg.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

branch.o:branch.c inst.h env.h cond.h aluop.h utils.h reg.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

cond.o:cond.c cond.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

function.o:function.c function.h env.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

aluop.o:aluop.c inst.h env.h cond.h aluop.h reg.h load_store.h utils.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

reg.o:reg.c reg.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

mem.o:mem.c mem.h env.h inst.h cond.h aluop.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

load_store.o:load_store.c inst.h env.h cond.h aluop.h mem.h reg.h utils.h branch.h
	gcc -Wall -c -o $@ $< $(CFLAGS)

emulator:$(OBJ) main.c inst.h env.h
	gcc -Wall -o $@ $^ $(CFLAGS) -L$(LIBDIR) -l$(LIBS)

clean:
	rm -rf $(TARGET) *.o
