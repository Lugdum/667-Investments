#UI Makefile                                                                    
                                                                                
CPPFLAGS =   `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -MMD
CC = gcc                                                                        
CFLAGS = -Wall -Wextra -std=c99 -O3 -g                                 
LDFLAGS = $(PTHREAD) $(GTKLIB) -export-dynamic                              
LDLIBS = `pkg-config --libs sdl` `pkg-config --libs gtk+-3.0` -lm -lcurl -lpthread -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
                                                                                
SRC = UI/UI.c ValueGrabber/fetcher.c UI/graph2/SDL_graph.c                                                             
OBJ = ${SRC:.c=.o}                                                              
DEP = ${SRC:.c=.d}                                                              
                                                                                
all: main                                                                         
                                                                                
main: UI/UI.o ValueGrabber/fetcher.o UI/graph2/SDL_graph.o                                                                    

-include ${DEP}                                                                 
                                                                                
.PHONY: clean                                                                   
                                                                                
clean:
	${RM} ${OBJ} #remove object files
	${RM} ${DEP} #remove depedency files
	${RM} main	 #remove main program
	${RM} -r cache
	${RM} *txt
	${RM} main.d

#END