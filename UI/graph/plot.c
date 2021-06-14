#include <stdio.h>
#include <stdlib.h>
#include "pbPlots.h"
#include "supportLib.h"
#include <time.h>
#include "plot.h"
#include <err.h>
#include <pthread.h>
#include "../../ValueGrabber/fetcher.h"
#include "../../Algorithms/bot.h"

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

int main(int argc, char *argv[])
{
  FILE *f = fopen("hist", "r");
  int hist[200];
  fread(hist, sizeof(int), 200, f);
  fclose(f);

  int len = 0;
  while(hist[len] != '\0' || hist[len] != 0)
    len++;
  printf("graph: ");
  for (int i = 0; i < len; i++)
    printf("%d, ", hist[i]);
  printf("\n");
  int point = len;
  if (len < 4)
    return EXIT_FAILURE;

	double xs [point];
	double ys [point];

  xs[0] = 0;
  for(int i = 1; i < point-1; i++)
    xs[i] = xs[i-1]+5;

  printf("\ngraph: ");
  for(int i = 0; i < point; i++)
  {
    printf("%d | ", *(hist+i));
    ys[i] = *(hist+i);
  }

  double y[point];
  for(int i = point-2, j = 0; i >= 0; i--, j++)
    {
      y[j] = ys[i];
    }

	RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
	DrawScatterPlot(canvasReference, 600, 400, xs, point-1, y, point-1);

	size_t length;
	double *pngdata = ConvertToPNG(&length, canvasReference->image);
	WriteToFile(pngdata, length, "graph.png");
	DeleteImage(canvasReference->image);

	return EXIT_SUCCESS;
}