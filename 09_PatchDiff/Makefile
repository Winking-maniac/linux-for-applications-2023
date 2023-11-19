CC=gcc
.RECIPEPREFIX=~

all: run

run: maze maze1 maze2 maze3
~ ./maze
~ ./maze1 6
~ ./maze2 ".#" 6
~ ./maze3 5 ".#" 6

maze1.c: maze.c patch1
~ cp maze.c maze1.c
~ patch maze1.c <patch1

maze2.c: maze1.c patch2
~ cp maze1.c maze2.c
~ patch maze2.c <patch2

maze3.c: maze2.c patch3
~ cp maze2.c maze3.c
~ patch maze3.c <patch3

clean:
~ rm -f maze maze1 maze2 maze3 maze1.c maze2.c maze3.c
    
