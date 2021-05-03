/* Autres includes */
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL2/SDL.h"

#define W 800
#define H 600

#define X_MIN -2.0
#define X_MAX 2.0

#define Y_MIN -3.0
#define Y_MAX 3.0

#define X_COEFF W / (X_MAX - X_MIN)
#define Y_COEFF H / (Y_MAX - Y_MIN)

#define PAS 1000

double valeurs(double x)
{
    return (2*cos(x) * sin(x) + x*x / 3);
}

void color_window(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void close(void)
{
    SDL_Event e;
    while(SDL_WaitEvent(&e) && e.type != SDL_QUIT)
        sleep(0.1);
}

void draw_axis(SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer, -X_MIN * X_COEFF, 0, -X_MIN * X_COEFF, H);
    SDL_RenderDrawLine(renderer, 0, Y_MAX * Y_COEFF, W, Y_MAX * Y_COEFF);
}

void draw_function(SDL_Renderer *renderer)
{
    double pas = (X_MAX - X_MIN) / PAS, x_prec = X_MIN, y_prec = valeurs(X_MIN);
    for(size_t i = 0; i < PAS; i++)
    {
        double x = x_prec + pas, y = valeurs(x);
        SDL_RenderDrawLine(renderer, (x_prec - X_MIN) * X_COEFF, (Y_MAX - y_prec) * Y_COEFF,
                                     (x - X_MIN) * X_COEFF, (Y_MAX - y) * Y_COEFF);
        x_prec = x;
        y_prec = y;
    }
}

void run(SDL_Renderer *renderer)
{
    SDL_Color blanc = {255, 255, 255, 255}, noir = {0, 0, 0, 255};
    color_window(renderer, blanc);
    SDL_SetRenderDrawColor(renderer, noir.r, noir.g, noir.b, noir.a);
    draw_axis(renderer);
    draw_function(renderer);
    SDL_RenderPresent(renderer);
    close();
}

int main(void)
{
    int statut = EXIT_FAILURE;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        fprintf(stderr, "Erreur SDL_Init : %s.", SDL_GetError());
    else
    {
        SDL_Window *window = SDL_CreateWindow("Fun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              W, H, SDL_WINDOW_SHOWN);
        if(!window)
            fprintf(stderr, "Erreur SDL_CreateWindow : %s.", SDL_GetError());
        else
        {
            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(!renderer)
                fprintf(stderr, "Erreur SDL_CreateRenderer : %s.", SDL_GetError());
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