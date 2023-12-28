GENERATES = range
TRASH = *.o test_*.ans
CC = gcc
.RECIPEPREFIX = ~
CFLAGS = -O0 -g

all: range

test: range 
~ gdb --batch --quiet -x test_1 ./range | grep -E "^start|^step|^stop|^i" | cat >test_1.ans 
~ gdb --batch --quiet -x test_2 ./range | grep -E "^start|^step|^stop|^i" | cat >test_2.ans 
~ cmp test_1.ans test_1.cor
~ cmp test_2.ans test_2.cor

clean:
~ rm -f $(TRASH)
~ rm -rf $(GENERATES)
