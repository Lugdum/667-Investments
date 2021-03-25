#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void initializePlotting() {
  Py_Initialize();
  // load matplotlib for plotting
  PyRun_SimpleString(
    "from matplotlib import pyplot as plt\n"
    "plt.ion()\n"
    "plt.show(block=False)\n"
    );
}

void uninitializePlotting() {
  PyRun_SimpleString("plt.ioff()\nplt.show()");
  Py_Finalize();
}

void plotPoint2d(double x, double y) {
#define CMD_BUF_SIZE 256
  static char command[CMD_BUF_SIZE];
  snprintf(command, CMD_BUF_SIZE, "plt.plot([%f],[%f],'r.')", x, y);
  PyRun_SimpleString(command);
  PyRun_SimpleString("plt.gcf().canvas.flush_events()");
}

double myRandom() {
  double sum = .0;
  int count = 1e4;
  int i;
  for (i = 0; i < count; i++)
    sum = sum + rand()/(double)RAND_MAX;
  sum = sum/count;
  return sum;
}

int main (int argc, const char** argv) {
  bool plot = false;
  if (argc == 2 && strcmp(argv[1], "--plot-data") == 0)
    plot = true;

  if (plot) initializePlotting();

  // generate and plot the data
  int i = 0;
  for (i = 0; i < 100; i++) {
    double x = myRandom(), y = myRandom();
    if (plot) plotPoint2d(x,y);
    else printf("%f %f\n", x, y);
  }

  if (plot) uninitializePlotting();
  return 0;
}