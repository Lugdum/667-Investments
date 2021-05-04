/* Autres includes */
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL2/SDL.h"
#include "../../ValueGrabber/fetcher.h"
#include "../UI.h"

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

double valeurs(double x)
{
    return (2*cos(x) * sin(x) + x*x / 3);
}

void color_window(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void stop(void)
{
    SDL_Event e;
    while(SDL_WaitEvent(&e) && e.type != SDL_QUIT)
        sleep(0.1);
}

void draw_background_lines(SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer, -X_MIN * X_COEFF, 0, -X_MIN * X_COEFF, H);
    SDL_RenderDrawLine(renderer, 0, Y_MAX * Y_COEFF, W, Y_MAX * Y_COEFF);
}

void draw_function(SDL_Renderer *renderer)
{
    int* hist = histo(btc);
    int i = 0;
    while(*(hist+i) != NULL)
        i++;
    int j = 0;
    while(i != 1)
    {
        SDL_RenderDrawLine(renderer, 10*j, *(hist+i), 10*j+10, *(hist+i-1));
        i--;
        j++;
    }
    /*double pas = (X_MAX - X_MIN) / PAS, x_prec = X_MIN, y_prec = valeurs(X_MIN);
    for(size_t i = 0; i < PAS; i++)
    {
        double x = x_prec + pas, y = valeurs(x);
        SDL_RenderDrawLine(renderer, (x_prec - X_MIN) * X_COEFF, (Y_MAX - y_prec) * Y_COEFF,
                                     (x - X_MIN) * X_COEFF, (Y_MAX - y) * Y_COEFF);
        x_prec = x;
        y_prec = y;
    }*/
}

void run(SDL_Renderer *renderer)
{
    SDL_Color white = {255, 255, 255, 255}, black = {0, 0, 0, 255};
    color_window(renderer, white);
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    draw_background_lines(renderer);
    draw_function(renderer);
    SDL_RenderPresent(renderer);
    stop();
}

int main(void)
{
    int statut = EXIT_FAILURE;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        fprintf(stderr, "Error SDL_Init: %s.", SDL_GetError());
    else
    {
        SDL_Window *window = SDL_CreateWindow("Fun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              W, H, SDL_WINDOW_SHOWN);
        if(!window)
            fprintf(stderr, "Error SDL_CreateWindow: %s.", SDL_GetError());
        else
        {
            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(!renderer)
                fprintf(stderr, "Error SDL_CreateRenderer: %s.", SDL_GetError());
            else
            {
                run(renderer);
                statut = EXIT_SUCCESS;
                SDL_DestroyRenderer(renderer);
            }
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }
    return statut;
}