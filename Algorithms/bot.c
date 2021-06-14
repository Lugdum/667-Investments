#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>
#include <err.h>
#include <pthread.h>
#include "SDL/SDL.h"
#include "../ValueGrabber/fetcher.h"
#include "../UI/UI.h"

int hist_len = 0;
int nb_val1 = 5;
int nb_val2 = 20;
int indic[3] = {-1, -1, -1};
float volume = 100;

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
  float         limit;
  struct Money  *next;
};

int* get_history(struct Money *money)
{
  hist_len++;
  int* hist = calloc(hist_len, sizeof(int));
  int i = 0;
  printf("\nvalue %d: %f\n", hist_len, money->priceUsd);
  while(i < hist_len)
  {
    *(hist + i) = money->priceUsd;
    //printf(" ,%f", money->priceUsd);
    money = money->next;
    i++;
  }
  //printf("\n\n");
  return hist;
}

float moyen(struct Money *money, int nb_val)
{
    float sum = 0;
    for (int i = 0; i < nb_val; i++)
    {
        if (money == NULL)
            return -1;
        sum += money->priceUsd;
        money = money->next;
    }
    return sum/(float)nb_val;
}

void average_crossover(struct Money *money)
{
    // Le moving average trading consiste à acheter si les valeurs dépasses la courbe des moyennes 
    // ou de vendre si elles passent en dessous.
    // indic permet de savoir si les dernières valeur étaient inférieur ou non à la courbe des moyennes sur nb_val valeurs.
    float av1 = moyen(money, nb_val1);
    float av2 = moyen(money, nb_val2);
    int i = -1;
    if (strcmp(money->symbol,"BTC") == 0)
        i = 0;
    else if (strcmp(money->symbol,"ETH") == 0)
        i = 1;
    else if (strcmp(money->symbol,"DOGE") == 0)
        i = 2;
    printf("Average5: %f    Average20: %f   Indic: %d\n", av1, av2, indic[i]);
    if (av1 > 0 && av2 > 0)
    {    
        if (indic[i] == -1 && av1 > av2)
        {
            indic[i] = 1;
            if (wallet_value >= volume)
            {
                printf("AUTO BUYING\n");
                buy(money, volume);
            }
        }
        else if (indic[i] == 1 && av1 < av2)
        {
            indic[i] = -1;
            printf("AUTO SELLING\n");
            sell(money, money->usd_possess);
        }

    }
}



