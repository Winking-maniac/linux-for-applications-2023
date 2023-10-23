GENERATES = esub esub.out sed.out
TRASH = *.o *.out
CC = gcc
.RECIPEPREFIX = ~

REGEX=[0-9]+([0-9])
SUBST=<\1>
STR=asdd344dgdf12sdf56


all: esub

test: esub
~ echo ${STR} | sed -E 's/${REGEX}/${SUBST}/' >sed.out
~ ./esub '${REGEX}' '${SUBST}' '${STR}' >esub.out
~ cmp esub.out sed.out

clean:
~ rm -f $(TRASH)
~ rm -rf $(GENERATES)
