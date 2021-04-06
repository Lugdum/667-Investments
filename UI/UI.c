#include <sys/types.h>
#include <sys/stat.h>
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

GtkWidget *window;//we set global variables

GtkWidget *quit_button;

GtkWidget *btc_graph_button;
GtkWidget *eth_graph_button;
GtkWidget *doge_graph_button;

GtkWidget *buy_button;
GtkWidget *sell_button;
GtkWidget *value_entry;

GtkWidget *btc_possess;
GtkWidget *eth_possess;
GtkWidget *doge_possess;

GtkWidget *value_label;
GtkWidget *total_money_label;

GtkBuilder *builder;

//on init la value du wallet à 100, et on créer la char* utilisé dans sprintf
int wallet_value = 100;
char val_txt[sizeof(int)];

volatile int on_money = 0;

char *amount;

void on_quit_button_clicked()
{
  gtk_main_quit();
}

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

void on_btc_graph_button_toggled()
{
  on_money = 0;
  //gboolean B = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
  if (1)
    {
      struct Money *strc = get_strc("bitcoin");
      float val = strc->priceUsd;
  
      char array[100];
      sprintf(array, "%f", val);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
    }
}

void on_eth_graph_button_toggled()
{
  on_money = 1;
  //gboolean B = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
  if (1)
    {
      struct Money *strc = get_strc("ethereum");
      float val = strc->priceUsd;
      
      char array[100];
      sprintf(array, "%f", val);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
    }
}  

void on_doge_graph_button_toggled()
{
  // Argument was "GtkRadioButton *b"
  on_money = 2;
  //gboolean B = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
  if (1) // was B
    {
      struct Money *strc = get_strc("dogecoin");
      float val = strc->priceUsd;
      
      char array[100];
      sprintf(array, "%f", val);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
    }
} 


char* on_value_entry_changed(GtkEntry *e)
{
  amount = gtk_entry_get_text(e);
}

void change_crypt_amount(char *crypt)
{
  struct Money *strc = get_strc(crypt);
  float btc_price = strc->priceUsd;
  char array[100];
  sprintf(array, "%f", val);
  //TODO
  gtk_label_set_text(GTK_LABEL(***_possess), (gchar*)array);

void on_buy_button_clicked()
{
  switch (on_money)
    {
      case 0:
	change_crypt_amount("bitcoin");
        break;
      case 1:
        change_crypt_amount("ethereum");
        break;
      case 2:
        change_crypt_amount("dogecoin");
        break;
    
      default:
        break;
    }
  struct Money *strc = get_strc("dogecoin");
  float btc_price = strc->priceUsd;
  

void on_sell_button_clicked();

void on_value_entry();

void on_btc_possess();

void on_eth_possess();

void on_doge_possess();


void loop()
{
  int i = 0;
  while(1)
  {
    sleep(4);
    //printf("\nupdating value %d\n\n", i);
    update_value("bitcoin");
    update_value("ethereum");
    update_value("dogecoin");
    sleep(1);
    switch (on_money)
    {
      case 0:
        on_btc_graph_button_toggled();
        break;
      case 1:
        on_eth_graph_button_toggled();
        break;
      case 2:
        on_doge_graph_button_toggled();
        break;
    
      default:
        break;
    }
    i++;
  }
}


int main()
{
  gtk_init(NULL,NULL); //we initialize the interface

  builder = gtk_builder_new_from_file("UI.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder,"window")); //we pick up all the widget boxes

  g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder,NULL);
 


  //Can touch
  quit_button = GTK_WIDGET(gtk_builder_get_object(builder,"quit_button"));
  btc_graph_button = GTK_WIDGET(gtk_builder_get_object(builder,"btc_graph_button"));
  eth_graph_button = GTK_WIDGET(gtk_builder_get_object(builder,"eth_graph_button"));
  doge_graph_button = GTK_WIDGET(gtk_builder_get_object(builder,"doge_graph_button"));
  buy_button = GTK_WIDGET(gtk_builder_get_object(builder,"buy_button"));
  sell_button = GTK_WIDGET(gtk_builder_get_object(builder,"sell_button"));
 
  value_entry = GTK_WIDGET(gtk_builder_get_object(builder,"value_entry"));

  btc_possess = GTK_WIDGET(gtk_builder_get_object(builder,"btc_possess"));

  eth_possess= GTK_WIDGET(gtk_builder_get_object(builder,"eth_possess"));

  doge_possess= GTK_WIDGET(gtk_builder_get_object(builder,"doge_possess"));

  value_label= GTK_WIDGET(gtk_builder_get_object(builder,"value_label"));

  pthread_t thread;
  int  iret;
  iret = pthread_create(&thread, NULL, (void*)loop, NULL);
  if (iret != 0)
    errx(EXIT_FAILURE, "ca bug mais c'est les thread donc c'est chelou");
  sleep(1);
  
  // permet de directement montrer la valeur du BTC
  struct Money *strc = get_strc("bitcoin");
  float val = strc->priceUsd;
  char array[100];
  sprintf(array, "%f", val);
  gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
  //gtk_label_set_text(GTK_LABEL(value_label), (gchar*)"1 BTC = xxx$");
 
  total_money_label= GTK_WIDGET(gtk_builder_get_object(builder,"total_money_label"));

  // on convertie wallet_value en char* pour pouvoir etre mis dans un label
  sprintf(val_txt, "%d", wallet_value);
  gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);

 
  //Can't touch
  gtk_widget_show(window);

  gtk_main();
  
  return EXIT_SUCCESS;
}
