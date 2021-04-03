#UI Makefile
CC=gcc
CPPFLAGS=   `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra  -std=c99 -O3
LDFLAGS= $(PTHREAD) $(GTKLIB) -export-dynamic
LDLIBS= `pkg-config --libs sdl` `pkg-config --libs gtk+-3.0` -lSDL_image -lm 


SRC= ui.c 
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}

all: ui

gui:${OBJ}


ui: ui.o 



clean:
    ${RM} ${OBJ} #remove object files
    ${RM} ${DEP} #remove depedency files
    ${RM} ui    #remove main program
    ${RM} -r cache




-include ${DEP}
#END

# This makefile is use to test the preprocessing step of our program.
# 
