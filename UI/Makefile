#UI Makefile                                                                    
                                                                                
CPPFLAGS =   `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -MMD
CC = gcc                                                                        
CFLAGS = -Wall -Wextra -std=c99 -O3 -g                                 
LDFLAGS = $(PTHREAD) $(GTKLIB) -export-dynamic                              
LDLIBS = `pkg-config --libs sdl` `pkg-config --libs gtk+-3.0` -lm -lcurl -lpthread -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
                                                                                
SRC = UI.c ../ValueGrabber/fetcher.c graph2/SDL_graph.c                                                             
OBJ = ${SRC:.c=.o}                                                              
DEP = ${SRC:.c=.d}                                                              
                                                                                
all: UI SDL                                                                         
                                                                                
UI: UI.o ../ValueGrabber/fetcher.o graph2/SDL_graph.o                                                                    
SDL: graph2/SDL_graph.c

-include ${DEP}                                                                 
                                                                                
.PHONY: clean                                                                   
                                                                                
clean:
	${RM} ${OBJ} #remove object files
	${RM} ${DEP} #remove depedency files
	${RM} UI	 #remove main program
	${RM} -r cache

#END