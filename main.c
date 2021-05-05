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
#include "SDL2/SDL.h"
#include "ValueGrabber/fetcher.h"
#include "UI/UI.h"

/*void launch_graph()
{
    launch();
}*/

int main()
{
    /*pthread_t thread;
    int  iret;
    iret = pthread_create(&thread, NULL, (void*)launch_graph, NULL);
    if (iret != 0)
        errx(EXIT_FAILURE, "ca bug mais c'est les thread donc c'est chelou");*/
    open_interface();
}