#include "../ValueGrabber/fetcher.c"
#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void initializegraphting() {
  Py_Initialize();
  //load matgraphlib for graphting
  PyRun_SimpleString(
    "from matgraphlib import pygraph as plt\n"
    "plt.ion()\n"
    "plt.show(block=False)\n"
    );
}

void uninitializegraphting() {
  PyRun_SimpleString("plt.ioff()\nplt.show()");
  Py_Finalize();
}

void graphPoint2d(double x, double y) {
#define CMD_BUF_SIZE 256
  static char command[CMD_BUF_SIZE];
  snprintf(command, CMD_BUF_SIZE, "plt.graph([%f],[%f],'r.')", x, y);
  PyRun_SimpleString(command);
  PyRun_SimpleString("plt.gcf().canvas.flush_events()");
}

char **remove_main(int argc, char ** argv)
{
  char** cpt_list = malloc((argc-1)*sizeof(char*));
  for (int i = 1; i < argc; i++)
    *(cpt_list+i-1) = argv[i];
  return cpt_list;
}

float getvalue(int argc, char** argv) {
  //struct Money *val = *get_strc_list(argc-1, &argv[1]);
  struct Money *val = *get_strc_list(argc, argv); //En gros ca marche mais le val.priceUsd renvoie -63456800000 bref n'importe quoi c'est bizarre
  //printf("%lf\n\n", val.priceUsd);
  printf("Bonjour%sBonjour\n\n", val->id);
  return val->priceUsd;
  return 10;
}

int main (int argc, char** argv) {
  argv = remove_main(argc, argv);
  argc--;
  double sec = 1;
  bool graph = false;
  if (argc == 2 && strcmp(argv[1], "--graph-data") == 0)
    graph = true;

  if (graph) initializegraphting();

  double x = 0, y;
  // generate and graph the data
  int i = 0;
  for (i = 0; i < 10; i++) {
    x += sec, y = getvalue(argc, argv);
    if (graph) graphPoint2d(x,y);
    else printf("%f %f\n", x, y);
  }

  if (graph) uninitializegraphting();
  return 0;
}