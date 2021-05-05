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
GtkWidget *l_x2_button;
GtkWidget *l_x5_button;
GtkWidget *l_x10_button;

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

int on_money = 0;

const char *amount;
const char *sl;

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
      char sl1[20] = "BTC SL = ";
      strcat(sl1, sl);
      gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)sl1);
      btc->limit = strtod(sl,NULL);
      break;
    case 1: ;
      char sl2[20] = "ETH SL = ";
      strcat(sl1, sl);
      gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)sl2);
      eth->limit = strtod(sl,NULL);
      break;
    case 2: ;
      char sl3[20] = "DOGE SL = ";
      strcat(sl1, sl);
      gtk_label_set_text(GTK_LABEL(sl_pending_label), (gchar*)sl3);
      doge->limit = strtod(sl,NULL);
      break;
    }
}

void change_crypt_amount(struct Money *strc)
{
    float temp = strtod(amount,NULL);
    if (pos > 0 && temp > wallet_value)
        //err(EXIT_FAILURE, "You don't have enough money.");
        return;
    if (pos < 0)
    {
        if (on_money == 0 && temp > btc->usd_possess)
            //err(EXIT_FAILURE, "You don't have enough Bitcoin.");
            return;
        if (on_money == 1 && temp > eth->usd_possess)
            //err(EXIT_FAILURE, "You don't have enough Ethereum.");
            return;
        if (on_money == 2 && temp > doge->usd_possess)
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
            btc->usd_possess += temp;
            btc->nb_possess = btc->usd_possess/strc->priceUsd;  
            //printf("new bitcoin amount is %f\n", btc->nb_possess);
            //printf("new bitcoin wallet is %f\n", btc->usd_possess);

            sprintf(array, "%f : %f$", btc->nb_possess, btc->usd_possess);
            gtk_label_set_text(GTK_LABEL(btc_possess), (gchar*)array);
            break;

        case 1:
            eth->usd_possess += temp;
            eth->nb_possess = eth->usd_possess/strc->priceUsd;  
            //printf("new ethereum amount is %f\n", eth->nb_possess);
            //printf("new ethereum wallet is %f\n", eth->usd_possess);

            sprintf(array, "%f : %f$", eth->nb_possess, eth->usd_possess);
            gtk_label_set_text(GTK_LABEL(eth_possess), (gchar*)array);
            break;

         case 2:
            doge->usd_possess += temp;
            doge->nb_possess = doge->usd_possess/strc->priceUsd;  
            //printf("new dogecoin amount is %f\n", doge->nb_possess);
            //printf("new dogecoin wallet is %f\n", doge->usd_possess);

            sprintf(array, "%f : %f$", doge->nb_possess, doge->usd_possess);
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
        change_crypt_amount(btc);
        break;
      case 1:
        change_crypt_amount(eth);
        break;
      case 2:
        change_crypt_amount(doge);
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
        btc->limit = 0;
        change_crypt_amount(btc);
        break;
      case 1:
        eth->limit = 0;
        change_crypt_amount(eth);
        break;
      case 2:
        doge->limit = 0;
        change_crypt_amount(doge);
        break;
    
      default:
        break;
    }
}

void on_value_entry();

void on_money_possess(int i_money)
{   
    struct Money *money;
    switch (i_money)
    {
        case 0: 
            money = btc;
            break;
        case 1:
            money = eth;
            break;
        case 2:
            money = doge;
            break;
        default:
            break;
    }

    char array[100];
    money->usd_possess = money->nb_possess*money->priceUsd;

    // STOPLOSS
    
    if (money->limit > 0 && money->usd_possess < money->limit)
    {
        int tmp = on_money;
        on_money = i_money;
        money->limit = 0;
       
        sprintf(amount, "%f", money->usd_possess);
        change_crypt_amount(money);
        
        on_money = tmp;
    }
    
    sprintf(array, "%f : %f$", money->nb_possess, money->usd_possess);
    gtk_label_set_text(GTK_LABEL(btc_possess), (gchar*)array);

}

void update_possess_money_price()
{
    if (btc->nb_possess > 0)
        on_money_possess(0);
    if (eth->nb_possess > 0)
        on_money_possess(1);
    if (doge->nb_possess > 0)
        on_money_possess(2);
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
  l_x2_button = GTK_WIDGET(gtk_builder_get_object(builder,"l_x2_button")); 
  l_x5_button = GTK_WIDGET(gtk_builder_get_object(builder,"l_x5_button"));
  l_x10_button = GTK_WIDGET(gtk_builder_get_object(builder,"l_x10_button"));

  
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
  sprintf(val_txt, "%d", wallet_value);
  gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);

 
  //Can't touch
  gtk_widget_show(window);

  gtk_main();
  
  return EXIT_SUCCESS;
}
