#include <stdio.h>
#include <stdlib.h>
#include "pbPlots.h"
#include "supportLib.h"
#include <time.h>
#include "../../ValueGrabber/fetcher.h"

struct Money
{
  char     *id;
  int      rank;
  char     *symbol;
  char     *name;
  float    supply;
  float    maxSupply;
  float    marketCapUsd;
  float    volumeUsd24Hr;
  float    priceUsd;
  float    changePercent24Hr;
  float    vwap24Hr;
};

int get_value()
{
    update_value("bitcoin");
    struct Money *strc = get_strc("bitcoin");
    float val = strc->priceUsd;
    return val;
}

int main()
{
  int point = 100;

  int y = get_value();
	double xs [point];
	double ys [point];

  xs[0] = 0;
  for(int i = 1; i < point; i++)
    xs[i] = xs[i-1]+5;

  srand(time(0));
  ys[0] = get_value();
  for(int i = 1; i < point; i++)
    {
      int pos = 1;
      if(rand()%2 == 0)
        pos = -1;
      ys[i] = ys[i-1] + (rand() % 100)*pos;
    }

	RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
	DrawScatterPlot(canvasReference, 600, 400, xs, point, ys, point);

	size_t length;
	double *pngdata = ConvertToPNG(&length, canvasReference->image);
	WriteToFile(pngdata, length, "graph.png");
	DeleteImage(canvasReference->image);

	return 0;
}