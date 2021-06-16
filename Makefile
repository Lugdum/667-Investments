#UI Makefile                                                                    
                                                                                
CPPFLAGS =   `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -MMD
CC = gcc                                                                        
CFLAGS = -Wno-error -std=c99 -O3 -g                                 
LDFLAGS = $(PTHREAD) $(GTKLIB) -export-dynamic                              
LDLIBS = `pkg-config --libs sdl` `pkg-config --libs gtk+-3.0` -lm -lcurl -lpthread -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
                                                                                
SRC = UI/UI.c ValueGrabber/fetcher.c Algorithms/bot.c #UI/graph/pbPlot.c UI/graph/supportLib.c  # UI/graph/plot.c                                                           
OBJ = ${SRC:.c=.o}                                                              
DEP = ${SRC:.c=.d}                                                              
                                                                                
all: main pl                                                                         
                                                                                
main: UI/UI.o ValueGrabber/fetcher.o Algorithms/bot.o #UI/graph/pbPlots.o UI/graph/supportLib.o #UI/graph/plot.o
	gcc -o main main.c UI/UI.c ValueGrabber/fetcher.c Algorithms/bot.c `pkg-config --cflags --libs gtk+-3.0` -lcurl `pkg-config --libs sdl` -rdynamic -std=c99 -O3 -g

pl: UI/UI.o ValueGrabber/fetcher.o Algorithms/bot.o UI/graph/pbPlots.o UI/graph/supportLib.o
	gcc -o pl UI/graph/plot.c UI/graph/pbPlots.c UI/graph/supportLib.c -lm

-include ${DEP}                                                                 
                                                                                
.PHONY: clean                                                                   
                                                                                
clean:
	${RM} ${OBJ} #remove object files
	${RM} ${DEP} #remove depedency files
	${RM} main	 #remove main program
	${RM} -r cache
	${RM} *txt
	${RM} main.d
	${RM} pl
	${RM} hist
	${RM} UI/graph/*.o
	${RM} UI/graph/*.d

#END