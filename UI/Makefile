#UI Makefile

CPPFLAGS =   `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -MMD
CC = gcc
CFLAGS = -Wall -Wextra  -std=c99 -O3
LDFLAGS = $(PTHREAD) $(GTKLIB) -export-dynamic
LDLIBS = `pkg-config --libs sdl` `pkg-config --libs gtk+-3.0` -lSDL_image -lm 

SRC = UI.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: UI

UI: UI.o

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ} #remove object files
	${RM} ${DEP} #remove depedency files
	${RM} UI    #remove main program
	${RM} -r cache

#END
