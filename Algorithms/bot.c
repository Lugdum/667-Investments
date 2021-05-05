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
  int *histo = calloc(1, sizeof(int));
  int i = 0;
  printf("\n\n");
  while(money->next != NULL)
  {
    histo = realloc(histo, (i+1)*sizeof(int));
    *(histo + i) = money->priceUsd;
    printf(" ,%f", money->priceUsd);
    money = money->next;
    i++;
  }
  printf("\n\n");
  return histo;
}