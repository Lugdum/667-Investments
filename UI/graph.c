#include "../ValueGrabber/fetcher.h"
#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void initializegraphting() {
  Py_Initialize();
  // load matgraphlib for graphting
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

double moneyvalue(int argc, char **argv) {
  return sum;
}

int main (int argc, const char** argv) {
  int s = atoi(argv[2]);
  bool graph = false;
  if (argc == 2 && strcmp(argv[1], "--graph-data") == 0)
    graph = true;

  if (graph)
    initializegraphting();

  // generate and graph the data
  int i = 0;
  double x = 0, y;
  for (i = 0; i < 100; i++) {
    x += s, y = moneyvalue(argc-1, argv);
    if (graph)
      graphPoint2d(x,y);
    else
      printf("%f %f\n", x, y);
    sleep(s);
  }

  if (graph)
    uninitializegraphting();
  return 0;
}