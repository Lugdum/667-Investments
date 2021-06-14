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
#include "../ValueGrabber/fetcher.h"
#include "../Algorithms/bot.h"

GtkWidget *window;//we set global variables

GtkWidget *quit_button;

GtkWidget *btc_graph_button;
GtkWidget *eth_graph_button;
GtkWidget *doge_graph_button;

GtkWidget *buy_button;
GtkWidget *sell_button;
GtkWidget *value_entry;

GtkWidget *sl_entry;
GtkWidget *sl_button;
GtkWidget *sl_pending_label;

GtkWidget *l_x1_button;
GtkWidget *l_x10_button;
GtkWidget *l_x50_button;
GtkWidget *l_x100_button;

GtkWidget *btc_possess;
GtkWidget *eth_possess;
GtkWidget *doge_possess;

GtkWidget *value_label;
GtkWidget *total_money_label;

GtkBuilder *builder;

GtkWidget *w_graph;


//initiation of the wallet's value to 1000 
float wallet_value = 1000;
char val_txt[8*sizeof(long)];
int pos = 1;
int lev=1;
int btc_lev=1;
int eth_lev=1;
int doge_lev=1;

int on_money = 0;

char *amount;
const char *sl;
float btc_init_pos;
float eth_init_pos;
float doge_init_pos;

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

void update_image()
{
  w_graph = GTK_WIDGET(gtk_builder_get_object(builder, "graph"));
  if(hist_len < 5)
    gtk_image_set_from_file(GTK_IMAGE(w_graph), (gchar*)"stonks.jpg");
  else
    gtk_image_set_from_file(GTK_IMAGE(w_graph), (gchar*)"graph.png");
}

void on_quit_button_clicked()
{
  gtk_main_quit();
}

void on_btc_graph_button_toggled()
{
  on_money = 0;
  if (1)
    {
      char array[100];
      sprintf(array, "%f", btc->priceUsd);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
    }
}

void on_eth_graph_button_toggled()
{
  on_money = 1;
  if (1)
    {
      char array[100];
      sprintf(array, "%f", eth->priceUsd);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
    }
}  

void on_doge_graph_button_toggled()
{
  // Argument was "GtkRadioButton *b"
  on_money = 2;
  if (1) // was B
    {
      char array[100];
      sprintf(array, "%f", doge->priceUsd);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
    }
} 


void on_value_entry_changed(GtkEntry *e)
{
  amount = gtk_entry_get_text(e);
}

void on_sl_entry_changed(GtkEntry *e)
{
  sl = gtk_entry_get_text(e);
}

void on_sl_button_clicked()
{
  switch (on_money)
    {
    case 0: ;
      if (strtod(sl, NULL) <= btc->usd_possess)
      {
        char sl1[20] = "BTC SL = ";
        strcat(sl1, sl);
        gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)sl1);
        btc->limit = strtod(sl,NULL);
      }
      break;
    case 1: ;
      if (strtod(sl, NULL) <= eth->usd_possess)
      {
        char sl2[20] = "ETH SL = ";
        strcat(sl2, sl);
        gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)sl2);
        eth->limit = strtod(sl,NULL);
      }
      break;
    case 2: ;
      if (strtod(sl, NULL) <= doge->usd_possess)
      {
        char sl3[20] = "DOGE SL = ";
        strcat(sl3, sl);
        gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)sl3);
        doge->limit = strtod(sl,NULL);
      }
      break;
    }
}

void on_l_x1_button_toggled()
{
  printf("x1 active\n");
  lev = 1;
}

void on_l_x10_button_toggled()
{
  printf("x10 active\n");
  lev = 10;
}

void on_l_x50_button_toggled()
{
  printf("x50 active\n");
  lev = 50;
}

void on_l_x100_button_toggled()
{
  printf("x100 active\n");
  lev = 100;
}

void set_lev(struct Money *money)
{
    if (strcmp(money->symbol, "BTC") == 0)
        btc_lev = lev;
    else if (strcmp(money->symbol, "ETH") == 0)
        eth_lev = lev;
    else if (strcmp(money->symbol, "DOGE") == 0)
        doge_lev = lev;
}

void change_crypt_amount(struct Money *strc, float volume)
{
    if (volume == -1)
        volume = atof(amount);
    printf("Money %s: %dx%f %f\n", strc->id, pos, volume, wallet_value);

    volume*=(float)pos;
    printf("New interaction with pos = %d\n", pos);
    total_money_label= GTK_WIDGET(gtk_builder_get_object(builder,"total_money_label"));
    char array[100];
    printf("temp = %f\n", volume);
    if (strcmp(strc->symbol, "BTC") == 0)
    {
        btc->usd_possess += volume;
        btc_init_pos = btc->usd_possess;
        btc->nb_possess = btc->usd_possess/strc->priceUsd;  
        printf("new bitcoin amount is %f\n", btc->nb_possess);
        printf("new bitcoin wallet is %f\n", btc->usd_possess);

        sprintf(array, "%f : %f$", btc->nb_possess, btc->usd_possess);
        gtk_label_set_text(GTK_LABEL(btc_possess), (gchar*)array);
    }

    if (strcmp(strc->symbol, "ETH") == 0)
    {
        eth->usd_possess += volume;
        eth_init_pos = eth->usd_possess;
        eth->nb_possess = eth->usd_possess/strc->priceUsd;  
        printf("new ethereum amount is %f\n", eth->nb_possess);
        printf("new ethereum wallet is %f\n", eth->usd_possess);

        sprintf(array, "%f : %f$", eth->nb_possess, eth->usd_possess);
        gtk_label_set_text(GTK_LABEL(eth_possess), (gchar*)array);
    }

    if (strcmp(strc->symbol, "DOGE") == 0)
    {
        doge->usd_possess += volume;
        doge_init_pos = doge->usd_possess;
        doge->nb_possess = doge->usd_possess/strc->priceUsd;  
        printf("new dogecoin amount is %f\n", doge->nb_possess);
        printf("new dogecoin wallet is %f\n", doge->usd_possess);

        sprintf(array, "%f : %f$", doge->nb_possess, doge->usd_possess);
        gtk_label_set_text(GTK_LABEL(doge_possess), (gchar*)array);
    }

    wallet_value -= volume;
    printf("new wallet amount is %f\n", wallet_value);
    sprintf(val_txt, "%f", wallet_value);
    gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);
    printf("DONE\n");
}

void buy(struct Money *strc, float n)
{
    printf("Buying %f %s...\n", n, strc->symbol);
    pos = 1;
    if (wallet_value >= n)
    {
        set_lev(strc);
        change_crypt_amount(strc, n);    
    }
}

void sell(struct Money *strc, float n)
{
    printf("Selling %f %s...\n", n, strc->symbol);
    pos = -1;
    set_lev(strc);
    change_crypt_amount(strc, n);
}



void on_buy_button_clicked()
{
    pos = 1;
    if (amount && atof(amount) <= wallet_value)
    {
        printf("BUYING with %d x%d\n", on_money, lev);
        switch (on_money)
        {
            case 0:
                set_lev(btc);
                change_crypt_amount(btc, -1);
                break;
            case 1:
                set_lev(eth);
                change_crypt_amount(eth, -1);
                break;
            case 2:
                set_lev(doge);
                change_crypt_amount(doge, -1);
                break;
            default:
                break;
        }
    }
}
  

void on_sell_button_clicked()
{
  pos = -1;
  if (amount)
  {
        float a = atof(amount);
        switch (on_money)
        {
            case 0:
                if (a <= btc->usd_possess)
                {
                    if (btc->usd_possess-a < btc->limit)
                        btc->limit = 0;
                    change_crypt_amount(btc, -1);
                }
                break;
            case 1:
                if (a <= eth->usd_possess)
                {
                    if (eth->usd_possess-a < eth->limit)
                        eth->limit = 0;
                    change_crypt_amount(eth, -1);
                }
                break;
            case 2:
                if (a <= doge->usd_possess)
                {
                    if (doge->usd_possess-a < doge->limit)
                        doge->limit = 0;
                    change_crypt_amount(doge, -1);
                }
                break;
            default:
                break;
        }
    
  }
}

void on_value_entry();

void on_money_possess(int i_money)
{   
    float init_pos = 0;
    char array[100];
    float newPrice;
    float lvlEffectPrice;
    switch (i_money)
      {
      case 0: 
    printf("USD possess = %f, nb_possess = %f\n", btc->usd_possess, btc->nb_possess);
    init_pos = btc_init_pos;
    newPrice = btc->nb_possess*btc->priceUsd;
    lvlEffectPrice = (newPrice-init_pos)*btc_lev;
    printf("Levier : %d\n", btc_lev);
    btc->usd_possess = init_pos+lvlEffectPrice;
    printf("Increase of %f\n", lvlEffectPrice);
    
    if (btc->limit > 0 && btc->usd_possess < btc->limit)
      {
        printf("STOPLOSS ACTION\n");
        btc->limit = 0;
        gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)"");
  
        pos = -1;
        change_crypt_amount(btc, btc->usd_possess);
        
        
      }
    sprintf(array, "%f : %f$", btc->nb_possess, btc->usd_possess);
    gtk_label_set_text(GTK_LABEL(btc_possess), (gchar*)array);
	break;
      case 1:
      printf("USD possess = %f, nb_possess = %f\n", eth->usd_possess, eth->nb_possess);
    init_pos = eth_init_pos;
    newPrice = eth->nb_possess*eth->priceUsd;
    lvlEffectPrice = (newPrice-init_pos)*eth_lev;
    printf("Levier : %d\n", eth_lev);
    eth->usd_possess = init_pos+lvlEffectPrice;
    printf("Increase of %f\n", lvlEffectPrice);
    
    if (eth->limit > 0 && eth->usd_possess < eth->limit)
      {
        eth->limit = 0;
        gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)"");
        printf("STOPLOSS ACTION");
  
        pos = -1;
        change_crypt_amount(eth, eth->usd_possess);
      }
    sprintf(array, "%f : %f$", eth->nb_possess, eth->usd_possess);
    gtk_label_set_text(GTK_LABEL(eth_possess), (gchar*)array);
	break;
      case 2:
      printf("USD possess = %f, nb_possess = %f\n", doge->usd_possess, doge->nb_possess);
    init_pos = doge_init_pos;
    newPrice = doge->nb_possess*doge->priceUsd;
    lvlEffectPrice = (newPrice-init_pos)*doge_lev;
    printf("Levier : %d\n", doge_lev);
    doge->usd_possess = init_pos+lvlEffectPrice;
    printf("Increase of %f\n", lvlEffectPrice);
    
    if (doge->limit > 0 && doge->usd_possess < doge->limit)
      {
        doge->limit = 0;
        gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)"");
        printf("STOPLOSS ACTION");
  
        pos = -1;
        change_crypt_amount(doge, doge->usd_possess);
      }
    sprintf(array, "%f : %f$", doge->nb_possess, doge->usd_possess);
    gtk_label_set_text(GTK_LABEL(doge_possess), (gchar*)array);
	break;
      default:
	break;
      }
}

void update_possess_money_price()
{
    if (btc->nb_possess > 0)
        on_money_possess(0);
    if (eth->nb_possess > 0)
        on_money_possess(1);
    if (doge->nb_possess > 0)
        on_money_possess(2);
    
    // Moving Average Crossover
    printf("\nBOT EVALUATION\n");
    average_crossover(btc);
    average_crossover(eth);
    average_crossover(doge);
}

void begin_loop()
{
  loop();
}

int open_interface()
{
  gtk_init(NULL,NULL); //we initialize the interface

  builder = gtk_builder_new_from_file("UI/UI.glade");

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

  sl_entry = GTK_WIDGET(gtk_builder_get_object(builder,"sl_entry"));
  sl_button = GTK_WIDGET(gtk_builder_get_object(builder,"sl_button"));
  sl_pending_label = GTK_WIDGET(gtk_builder_get_object(builder,"sl_pending_label"));

  l_x1_button = GTK_WIDGET(gtk_builder_get_object(builder,"l_x1_button"));
  l_x10_button = GTK_WIDGET(gtk_builder_get_object(builder,"l_x10_button")); 
  l_x50_button = GTK_WIDGET(gtk_builder_get_object(builder,"l_x50_button"));
  l_x100_button = GTK_WIDGET(gtk_builder_get_object(builder,"l_x100_button"));

  
  sell_button = GTK_WIDGET(gtk_builder_get_object(builder,"sell_button"));
  
  value_entry = GTK_WIDGET(gtk_builder_get_object(builder,"value_entry"));

  btc_possess = GTK_WIDGET(gtk_builder_get_object(builder,"btc_possess"));

  eth_possess= GTK_WIDGET(gtk_builder_get_object(builder,"eth_possess"));

  doge_possess= GTK_WIDGET(gtk_builder_get_object(builder,"doge_possess"));

  value_label= GTK_WIDGET(gtk_builder_get_object(builder,"value_label"));

  pthread_t thread;
  int  iret;
  iret = pthread_create(&thread, NULL, (void*)begin_loop, NULL);
  if (iret != 0)
    errx(EXIT_FAILURE, "ca bug mais c'est les thread donc c'est chelou");
  sleep(5);
  
  //show exactly the value of the BTC

  printf("TEST\n");
  char array[10000];
  sprintf(array, "%f", btc->priceUsd);
  gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
 
  total_money_label= GTK_WIDGET(gtk_builder_get_object(builder,"total_money_label"));
  printf("TEST\n");

  //we convert wallet_value  in char* to put in a label
  sprintf(val_txt, "%f", wallet_value);
  gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);

 
  //Can't touch
  gtk_widget_show(window);

  gtk_main();
  
  return EXIT_SUCCESS;
}
