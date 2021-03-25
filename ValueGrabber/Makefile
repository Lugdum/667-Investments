# Makefile

CPPFLAGS = -MMD
CC = gcc

SRC = fetcher.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: test

test: fetcher.o
	${CC} fetcher.o -lcurl -o test
fetcher.o: fetcher.c fetcher.h

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}

# END
