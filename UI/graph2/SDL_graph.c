/* Autres includes */
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "SDL2/SDL.h"
#include "../../ValueGrabber/fetcher.h"
//#include "../UI.h"

#define W 800
#define H 600

#define X_MIN -2.0
#define X_MAX 2.0

#define Y_MIN -3.0
#define Y_MAX 3.0

#define X_COEFF W / (X_MAX - X_MIN)
#define Y_COEFF H / (Y_MAX - Y_MIN)

struct Money
{
  char          *id;
  int           rank;
  char          *symbol;
  char          *name;
  float         supply;
  float         maxSupply;
  float         marketCapUsd;
  float         volumeUsd24Hr;
  float         priceUsd;
  float         changePercent24Hr;
  float         vwap24Hr;
  float         usd_possess;
  float         nb_possess;
  struct Money  *next;
};

struct Money *btc;

void color_window(SDL_Renderer *graph, SDL_Color color)
{
    SDL_SetRenderDrawColor(graph, color.r, color.g, color.b, color.a);
    SDL_RenderClear(graph);
}

void stop(void)
{
    SDL_Event e;
    while(SDL_WaitEvent(&e) && e.type != SDL_QUIT)
        sleep(0.1);
}

void draw_background_lines(SDL_Renderer *graph)
{
    SDL_RenderDrawLine(graph, -X_MIN * X_COEFF, 0, -X_MIN * X_COEFF, H);
    SDL_RenderDrawLine(graph, 0, Y_MAX * Y_COEFF, W, Y_MAX * Y_COEFF);
}

void draw_lines(SDL_Renderer *graph)
{
    //int* hist = get_histo(btc);
    int* hist = calloc(5, sizeof(int));
    *(hist) = 250;
    *(hist+1) = 220;
    *(hist+2) = 193;
    *(hist+3) = 268;
    *(hist+4) = 310;
    int i = 0;
    while(*(hist+i) != '\0')
        i++;
    int j = 0;
    while(i != 1)
    {
        SDL_RenderDrawLine(graph, 10*j, *(hist+i), 10*j+10, *(hist+i-1));
        i--;
        j++;
    }
}

void run(SDL_Renderer *graph)
{
    SDL_Color white = {255, 255, 255, 255}, black = {0, 0, 0, 255};
    color_window(graph, white);
    SDL_SetRenderDrawColor(graph, black.r, black.g, black.b, black.a);
    draw_background_lines(graph);
    draw_lines(graph);
    SDL_RenderPresent(graph);
    stop();
}

int main(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        errx(EXIT_FAILURE, "Error SDL_Init: %s.", SDL_GetError());
    
    SDL_Window *window = SDL_CreateWindow("Fun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
    if(!window)
        errx(EXIT_FAILURE, "Error SDL_CreateWindow: %s.", SDL_GetError());
    
    SDL_Renderer *graph = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!graph)
        errx(EXIT_FAILURE, "Error SDL_CreateRenderer: %s.", SDL_GetError());
   
    run(graph);

    SDL_DestroyRenderer(graph);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}