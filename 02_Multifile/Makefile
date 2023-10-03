GENERATES = prog prog-a prog-so README *.so
TRASH = *.o *~ o.* *.a *.test*
CC = gcc
CFLAGS= -fPIC
.RECIPEPREFIX = ~
LD_LIBRARY_PATH=.

all: README prog

test: test0 test1 test3

test%: run%_prog run%_prog-a run%_prog-so
~ cmp prog.$@ prog-a.$@ 
~ cmp prog.$@ prog-so.$@

run0_%: %
~ ln prog tmp
~ ./tmp > $<.test0 2>&1
~ unlink tmp

run1_%: %
~ ln prog tmp
~ ./tmp 1 > $<.test1 2>&1
~ unlink tmp

run3_%: %
~ ln prog tmp
~ ./tmp 1 2 3 > $<.test3 2>&1
~ unlink tmp


prog: const.o fun.o prog.o

prog-a: prog.o liboutput_static.a
~ ${CC} $< -L. -loutput_static -o $@

prog-so: prog.o liboutput.so
~ ${CC} $< -L. -loutput -o $@


README: prog
~ ./$< 2> $@

fun.o: outlib.h

liboutput_static.a: fun.o const.o
~ ar -rsc $@ $^

liboutput.so: fun.o const.o
~ ${CC} -shared $^ -o $@

clean:
~ rm -f $(TRASH)
~ rm -rf $(GENERATES)
