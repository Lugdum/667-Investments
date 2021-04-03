#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

GtkWidget *window;//we set global variables
GtkWidget *quit_button;

GtkBuilder *builder;


int main()
{
  gtk_init(NULL,NULL); //we initialize the interface

  builder = gtk_builder_new_from_file("UI.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder,"window")); //we pick up all the widget boxes


 g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL);

 gtk_builder_connect_signals(builder,NULL);


 //TODO
 quit_button = GTK_WIDGET(gtk_builder_get_object(builder,"quit_button"));

 gtk_widget_show(window);

 gtk_main();

 return EXIT_SUCCESS;
}

void on_quit_button_clicked()
{
  gtk_main_quit();
}
 
 






