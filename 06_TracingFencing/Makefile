GENERATES = move
TRASH = *.o *.so *.out output input.copy PROTECT_input.txt
CC = gcc
.RECIPEPREFIX = ~

all: move

test: move
~ chmod +x ./tests
~ ./tests

remove.so: remove.c
~ gcc -shared -o remove.so remove.c

ld: move remove.so
~ cp input.txt PROTECT_input.txt
~ LD_PRELOAD=$$PWD/remove.so ./move PROTECT_input.txt output || true
~ test -f PROTECT_input.txt

clean:
~ rm -f $(TRASH)
~ rm -rf $(GENERATES)
