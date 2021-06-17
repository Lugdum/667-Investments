/**
 * @author Julien Victor
 * @details Bot
 */

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

/** @brief Convert struct money to a list of history
 * @param[in] money     The money struct to convert in histo list
 * @return int*
 */
int* get_history(struct Money *money)
{
  hist_len++;
  int* hist = calloc(hist_len, sizeof(int));
  int i = 0;
  printf("\nmoney: %d\n", hist_len);
  while(i < hist_len)
  {
    *(hist + i) = money->priceUsd;
    printf(" ,%f", money->priceUsd);
    money = money->next;
    i++;
  }
  printf("\n\n");
  return hist;
}

/** @brief Write history list into a file
 * @param[in] money      The money struct to write histo into a file
 * @return void
 */
void write_history(struct Money *money)
{
  int* hist = get_history(money);
  FILE *f = fopen("hist", "wb");
  fwrite(hist, sizeof(int), 200, f);
  fclose(f);
  free(hist);
}

/** @brief Calculate the avrage of x values
 * @param[in] nb_val                  Number of value to avrage
 * @param[in] money         The money struct
 * @return float
 */
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

/** @brief The bot function
 * @param[in] money      The money struct to work on
 * @return void
 */
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
    else if (strcmp(money->symbol,"YFI") == 0)
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