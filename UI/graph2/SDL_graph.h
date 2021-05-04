
#ifndef SDL_GRAPH_H
#define SDL_GRAPH_H
#include "SDL2/SDL.h"

double valeurs(double x);
void run(SDL_Renderer *renderer);
void draw_background_lines(SDL_Renderer *renderer);
void draw_function(SDL_Renderer *renderer);
void set_window_color(SDL_Renderer *renderer, SDL_Color color);
void launch();

#endif