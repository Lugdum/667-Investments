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

double *moy(double y[], int point, int val)
{
  double z[val];
  if (point < val)
  {
    for (int i = 0; i < point; i++)
    {
      z[i] = y[i];
    }
  }
  else
  {
    int sum = 0;
    int i = 0;
    while (i < val)
    {
      z[i] = y[i];
      sum += y[i];
      i++;
    }
    while (i < point)
    {
      z[i] = sum/val;
      sum += y[i];
      sum -= y[i-val];
    }
  }
  return z;
}

// executable qui transforme l'image du graph a partir des valeurs
int main(int argc, char *argv[])
{
  // lis le fichier ou est l'historique
  FILE *f = fopen("hist", "r");
  int hist[200];
  fread(hist, sizeof(int), 200, f);
  fclose(f);

  // recupere sa longueur
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

  // cree deux tableaux
	double xs [point];
	double ys [point];

  // initialise le premier a 0, 5, 10 c'est le temps
  xs[0] = 0;
  for(int i = 1; i < point-1; i++)
    xs[i] = xs[i-1]+5;

  // le second c'est les valeurs
  printf("\ngraph: ");
  for(int i = 0; i < point; i++)
  {
    printf("%d | ", *(hist+i));
    ys[i] = *(hist+i);
  }

  // mais faut l'inverser
  double y[point];
  for(int i = point-2, j = 0; i >= 0; i--, j++)
    {
      y[j] = ys[i];
    }

  // truc moyenne casse couilles
  double z[point];
  int val = 5;
  if (point < val)
  {
    for (int i = 0; i < point; i++)
    {
      z[i] = y[i];
    }
  }
  else
  {
    int sum = 0;
    int i = 0;
    while (i < val)
    {
      z[i] = y[i];
      sum += y[i];
      i++;
    }
    while (i < point)
    {
      z[i] = sum/val;
      sum += y[i];
      sum -= y[i-val];
      i++;
    }
  }

  double w[point];
  val = 20;
  if (point < val)
  {
    for (int i = 0; i < point; i++)
    {
      w[i] = y[i];
    }
  }
  else
  {
    int sum = 0;
    int i = 0;
    while (i < val)
    {
      w[i] = y[i];
      sum += y[i];
      i++;
    }
    while (i < point)
    {
      w[i] = sum/val;
      sum += y[i];
      sum -= y[i-val];
      i++;
    }
  }

	RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();

  ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
	series->xs = xs;
	series->xsLength = point-1;
	series->ys = y;
	series->ysLength = point-1;
	series->lineThickness = 2;
	/*series->linearInterpolation = false;
	series->pointType = L"dots";
	series->pointTypeLength = wcslen(series->pointType);
	series->color = CreateRGBColor(1, 0, 0);*/

	ScatterPlotSeries *series2 = GetDefaultScatterPlotSeriesSettings();
	series2->xs = xs;
	series2->xsLength = point-1;
	series2->ys = z;
	series2->ysLength = point-1;
	/*series2->linearInterpolation = false;
	series2->lineType = L"solid";
	series2->lineTypeLength = wcslen(series->lineType);
	series2->lineThickness = 2;*/
	series2->color = CreateRGBColor(0, 1, 0);

  ScatterPlotSeries *series3 = GetDefaultScatterPlotSeriesSettings();
	series3->xs = xs;
	series3->xsLength = point-1;
	series3->ys = w;
	series3->ysLength = point-1;
	/*series2->linearInterpolation = false;
	series2->lineType = L"solid";
	series2->lineTypeLength = wcslen(series->lineType);
	series2->lineThickness = 2;*/
	series3->color = CreateRGBColor(0, 0, 1);

	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = L"";
	settings->titleLength = wcslen(settings->title);
	settings->xLabel = L"Time (s)";
	settings->xLabelLength = wcslen(settings->xLabel);
	settings->yLabel = L"Value ($)";
	settings->yLabelLength = wcslen(settings->yLabel);
	ScatterPlotSeries *s [] = {series2, series3, series};
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = 3;

	DrawScatterPlotFromSettings(imageReference, settings);
	/*RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
	DrawScatterPlot(canvasReference, 600, 400, xs, point-1, y, point-1);
	DrawScatterPlot(canvasReference, 600, 400, xs, point-1, z, point-1);*/

	size_t length;
	double *pngdata = ConvertToPNG(&length, imageReference->image);
	WriteToFile(pngdata, length, "graph.png");
	DeleteImage(imageReference->image);

	return EXIT_SUCCESS;
}