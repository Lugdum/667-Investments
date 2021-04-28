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


//initiation of the wallet's value to 1000 
int wallet_value = 1000;
char val_txt[8*sizeof(long)];
volatile int pos = 1;

volatile int on_money = 0;
volatile int open = 0;

const char *amount;

float usd_btc_possess = 0;
float usd_eth_possess = 0;
float usd_doge_possess = 0;
float nb_btc_possess = 0;
float nb_eth_possess = 0;
float nb_doge_possess = 0;

const char *limit;
float btc_limit;
float eth_limit;
float doge_limit;
volatile int active_stoplost = 0;

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
    while (open != 0)
        continue;
    struct Money *strc = get_strc("bitcoin");
    float val = strc->priceUsd;
  
    char array[100];
    sprintf(array, "%f", val);
    gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
}

void on_eth_graph_button_toggled()
{
    on_money = 1;
    while (open != 0)
        continue;
    struct Money *strc = get_strc("ethereum");
    float val = strc->priceUsd;
      
    char array[100];
    sprintf(array, "%f", val);
    gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
}  

void on_doge_graph_button_toggled()
{
    on_money = 2;
    while (open != 0)
        continue;
    struct Money *strc = get_strc("dogecoin");
    float val = strc->priceUsd;

    char array[100];
    sprintf(array, "%f", val);
    gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
} 


void on_value_entry_changed(GtkEntry *e)
{
  amount = gtk_entry_get_text(e);
}

void change_crypt_amount(char *crypt)
{
    while (open != 0)
        continue;
    struct Money *strc = get_strc(crypt);
    float temp = strtod(amount,NULL);
    if (pos > 0 && temp > wallet_value)
        //err(EXIT_FAILURE, "You don't have enough money.");
        return;
    if (pos < 0)
    {
        if (on_money == 0 && temp > usd_btc_possess)
            //err(EXIT_FAILURE, "You don't have enough Bitcoin.");
            return;
        if (on_money == 1 && temp > usd_eth_possess)
            //err(EXIT_FAILURE, "You don't have enough Ethereum.");
            return;
        if (on_money == 2 && temp > usd_doge_possess)
            //err(EXIT_FAILURE, "You don't have enough Dogecoin.");
            return;
    }

    temp*=pos;
    printf("New interaction with pos = %d\n", pos);
    total_money_label= GTK_WIDGET(gtk_builder_get_object(builder,"total_money_label"));
    char array[100];
    printf("temp = %f\n", temp);
    switch (on_money)
    {
        case 0:
            usd_btc_possess += temp;
            nb_btc_possess = usd_btc_possess/strc->priceUsd;  
            //printf("new bitcoin amount is %f\n", nb_btc_possess);
            //printf("new bitcoin wallet is %f\n", usd_btc_possess);

            sprintf(array, "%f : %f$", nb_btc_possess, usd_btc_possess);
            gtk_label_set_text(GTK_LABEL(btc_possess), (gchar*)array);
            break;

        case 1:
            usd_eth_possess += temp;
            nb_eth_possess = usd_eth_possess/strc->priceUsd;  
            //printf("new ethereum amount is %f\n", nb_eth_possess);
            //printf("new ethereum wallet is %f\n", usd_eth_possess);

            sprintf(array, "%f : %f$", nb_eth_possess, usd_eth_possess);
            gtk_label_set_text(GTK_LABEL(eth_possess), (gchar*)array);
            break;

         case 2:
            usd_doge_possess += temp;
            nb_doge_possess = usd_doge_possess/strc->priceUsd;  
            //printf("new dogecoin amount is %f\n", nb_doge_possess);
            //printf("new dogecoin wallet is %f\n", usd_doge_possess);

            sprintf(array, "%f : %f$", nb_doge_possess, usd_doge_possess);
            gtk_label_set_text(GTK_LABEL(doge_possess), (gchar*)array);
            break;
    }

    wallet_value -= temp;
    sprintf(val_txt, "%d", wallet_value);
    gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);
}

void on_buy_button_clicked()
{
  pos = 1;
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
}
  

void on_sell_button_clicked()
{
  pos = -1;
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
}

// JOJO !!! il faut lier la variable lim a l'interface stp
// c'est la quantité en usd de crypto limite pour le stoplost
// si tu peux faire un truc similaire a la const char* amount
// Mais il faudrais aussi une notion de bool pour activer et desactive le stoplost genre un truc a cocher
void on_stoplost_button_clicked(GtkEntry *lim)
{
    active_stoplost++;
    active_stoplost%=2;
    
    if (active_stoplost == 1)
    {
        limit = gtk_entry_get_text(lim);
        switch (on_money)
        {
            case 0:
                btc_limit = strtod(limit,NULL);
                break;
            case 1:
                eth_limit = strtod(limit,NULL);
                break;
            case 2:
                doge_limit = strtod(limit,NULL);
                break;
        }
    }
}

void on_value_entry();

void on_btc_possess()
{
    char array[100];
    while (open != 0)
        continue;
    struct Money *btc = get_strc("bitcoin");
    usd_btc_possess = nb_btc_possess*btc->priceUsd;
    sprintf(array, "%f : %f$", nb_btc_possess, usd_btc_possess);
    gtk_label_set_text(GTK_LABEL(btc_possess), (gchar*)array);
}
void on_eth_possess()
{
    char array[100];
    while (open != 0)
        continue;
    struct Money *eth = get_strc("ethereum");
    usd_eth_possess = nb_eth_possess*eth->priceUsd;
    sprintf(array, "%f : %f$", nb_eth_possess, usd_eth_possess);
    gtk_label_set_text(GTK_LABEL(eth_possess), (gchar*)array);
}

void on_doge_possess()
{
    char array[100];
    while (open != 0)
        continue;
    struct Money *doge = get_strc("dogecoin");
    usd_doge_possess = nb_doge_possess*doge->priceUsd;
    sprintf(array, "%f : %f$", nb_doge_possess, usd_doge_possess);
    gtk_label_set_text(GTK_LABEL(doge_possess), (gchar*)array);
}

void update_possess_money_price()
{
    if (nb_btc_possess > 0)
    {
        on_btc_possess();
        if (active_stoplost == 1 && usd_btc_possess < btc_limit)
        {
            on_money = 0;
            on_sell_button_clicked();    
        }
    }
    
    if (nb_eth_possess > 0)
    {
        on_eth_possess();
        if (active_stoplost == 1 && usd_eth_possess < eth_limit)
        {
            on_money = 1;
            on_sell_button_clicked();    
        }
    }

    if (nb_doge_possess > 0)
    {
        on_doge_possess();
        if (active_stoplost == 1 && usd_doge_possess < doge_limit)
        {
            on_money = 2;
            on_sell_button_clicked();
        }
    }
}

void loop()
{
  int i = 0;
  while(1)
  {
    sleep(5);
    open++;
    update_value("bitcoin");
    update_value("ethereum");
    update_value("dogecoin");
    update_possess_money_price();
    open--;
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
printf("WHY?\n");
  gtk_init(NULL, NULL); //we initialize the interface
printf("WHY?\n");
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

  printf("TEST\n");
  pthread_t thread;
  int  iret;
  iret = pthread_create(&thread, NULL, (void*)loop, NULL);
  if (iret != 0)
    errx(EXIT_FAILURE, "ca bug mais c'est les thread donc c'est chelou");
  sleep(2);
  
  //show exactly the value of the BTC

  while (open != 0)
      continue;
  struct Money *strc = get_strc("bitcoin");
  printf("TEST\n");
  float val = strc->priceUsd;
  char array[10000];
  sprintf(array, "%f", val);
  gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
 
  total_money_label= GTK_WIDGET(gtk_builder_get_object(builder,"total_money_label"));
  printf("TEST\n");

  //we convert wallet_value  in char* to put in a label
  sprintf(val_txt, "%d", wallet_value);
  gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);

 
  //Can't touch
  gtk_widget_show(window);

  gtk_main();
  
  return EXIT_SUCCESS;
}
