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

int* get_histo(struct Money *money)
{
  int *histo = calloc(1, sizeof(int));
  int i = 0;
  while(money->next != NULL)
  {
    histo = realloc(histo, (i+1)*sizeof(int));
    *(histo + i) = money->priceUsd;
    money = money->next;
    i++;
  }
  return histo;
}