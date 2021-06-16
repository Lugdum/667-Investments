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

GtkWidget *btc_sl;
GtkWidget *eth_sl;
GtkWidget *doge_sl;

GtkWidget *sl_entry;
GtkWidget *sl_button;
GtkWidget *sl_pending_label;

GtkWidget *l_x1_button;
GtkWidget *l_x10_button;
GtkWidget *l_x50_button;
GtkWidget *l_x100_button;

GtkWidget *bot_button;

GtkWidget *btc_possess;
GtkWidget *eth_possess;
GtkWidget *doge_possess;

GtkWidget *value_label;
GtkWidget *total_money_label;
GtkWidget *ch24;

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

int manual = 1;

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

// affichage du stoplost
void update_stoploss_display()
{
    char buffer[100];
    printf("UPDATING STOPLOSS\n");

    sprintf(buffer, "%f", btc->limit);
    gtk_label_set_text(GTK_LABEL(btc_sl), (gchar*)buffer);
    
    sprintf(buffer, "%f", eth->limit);
    gtk_label_set_text(GTK_LABEL(eth_sl), (gchar*)buffer);

    sprintf(buffer, "%f", doge->limit);
    gtk_label_set_label(GTK_LABEL(doge_sl), (gchar*)buffer);
}

// update le graph
void update_image(int len)
{
  w_graph = GTK_WIDGET(gtk_builder_get_object(builder, "graph"));
  if(hist_len < 5)
    gtk_image_set_from_file(GTK_IMAGE(w_graph), (gchar*)"stonks.jpg");
  else
    gtk_image_set_from_file(GTK_IMAGE(w_graph), (gchar*)"graph.png");
}


void on_bot_button_clicked()
{
  printf("I CLICK ON manual BUTTON \n");
  if (manual == 1)
    manual = 0;
  else
    manual = 1;
}

void on_quit_button_clicked()
{
  gtk_main_quit();
}

// choix de la monnaie bitcoin
void on_btc_graph_button_toggled()
{
  on_money = 0;
  if (1)
    {
      char array[100];
      sprintf(array, "%0.2f", btc->priceUsd);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
      char array2[100];
      sprintf(array2, "Evolution per 24H = %0.3f%% ",btc->changePercent24Hr);
      gtk_label_set_text(GTK_LABEL(ch24), (gchar*)array2);
    }
}

// choix de la monnaie ethereum
void on_eth_graph_button_toggled()
{
  on_money = 1;
  if (1)
    {
      char array[100];
      sprintf(array, "%0.2f", eth->priceUsd);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
      char array2[100];
      sprintf(array2, "Evolution per 24H = %0.3f%% ",eth->changePercent24Hr);
      gtk_label_set_text(GTK_LABEL(ch24), (gchar*)array2);
    }
}  

// choix de la monnaie doge
void on_doge_graph_button_toggled()
{
  // Argument was "GtkRadioButton *b"
  on_money = 2;
  if (1) // was B
    {
      char array[100];
      sprintf(array, "%0.5f", doge->priceUsd);
      gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
      char array2[100];
      sprintf(array2, "Evolution per 24H = %0.3f%% ",doge->changePercent24Hr);
      gtk_label_set_text(GTK_LABEL(ch24), (gchar*)array2);
    }
} 

// nombre de monnaie entre
void on_value_entry_changed(GtkEntry *e)
{
  amount = gtk_entry_get_text(e);
}

// stop loss nb
void on_sl_entry_changed(GtkEntry *e)
{
  sl = gtk_entry_get_text(e);
  printf("STOPLOSS IS %s\n", sl);
}

// stop loss setup
void on_sl_button_clicked()
{
    if (sl)
    {
        float new_sl = atof(sl);
        switch (on_money)
        {
            case 0: 
                if (new_sl <= btc->priceUsd)
                    btc->limit = new_sl;
                break;
            case 1: ;
                if (new_sl <= eth->priceUsd)
                    eth->limit = new_sl;
                break;
            case 2: ;
                if (new_sl <= doge->priceUsd)
                    doge->limit = new_sl;
                break;
        }
        update_stoploss_display();        
    }   
}

// levier x1
void on_l_x1_button_toggled()
{
    printf("x1 active\n");
    lev = 1;
}

// levier x10
void on_l_x10_button_toggled()
{
    printf("x10 active\n");
    lev = 10;
}

// levier x50
void on_l_x50_button_toggled()
{
    printf("x50 active\n");
    lev = 50;
}

// levier x100
void on_l_x100_button_toggled()
{
    printf("x100 active\n");
    lev = 100;
}

// activer le levier
void set_lev(struct Money *money)
{
    if (strcmp(money->symbol, "BTC") == 0 && btc_lev == 1)
        btc_lev = lev;
    else if (strcmp(money->symbol, "ETH") == 0 && eth_lev == 1)
        eth_lev = lev;
    else if (strcmp(money->symbol, "DOGE") == 0 && doge_lev == 1)
        doge_lev = lev;
}

// modifie la valeur de monnaie possede
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
        if (btc->usd_possess == 0)
            btc_lev = 1;
        btc_init_pos = btc->usd_possess;
        btc->nb_possess = btc->usd_possess/strc->priceUsd;  
        printf("new bitcoin amount is %f\n", btc->nb_possess);
        printf("new bitcoin wallet is %f\n", btc->usd_possess);

        sprintf(array, "%f : %0.3f$", btc->nb_possess, btc->usd_possess);
        gtk_label_set_text(GTK_LABEL(btc_possess), (gchar*)array);
    }

    if (strcmp(strc->symbol, "ETH") == 0)
    {
        eth->usd_possess += volume;
        if (eth->usd_possess == 0)
            eth_lev = 1;
        eth_init_pos = eth->usd_possess;
        eth->nb_possess = eth->usd_possess/strc->priceUsd;  
        printf("new ethereum amount is %f\n", eth->nb_possess);
        printf("new ethereum wallet is %f\n", eth->usd_possess);

        sprintf(array, "%f : %0.3f$", eth->nb_possess, eth->usd_possess);
        gtk_label_set_text(GTK_LABEL(eth_possess), (gchar*)array);
    }

    if (strcmp(strc->symbol, "DOGE") == 0)
    {
        doge->usd_possess += volume;
        if (doge->usd_possess == 0)
            doge_lev = 1;
        doge_init_pos = doge->usd_possess;
        doge->nb_possess = doge->usd_possess/strc->priceUsd;  
        printf("new dogecoin amount is %f\n", doge->nb_possess);
        printf("new dogecoin wallet is %f\n", doge->usd_possess);

        sprintf(array, "%f : %0.3f$", doge->nb_possess, doge->usd_possess);
        gtk_label_set_text(GTK_LABEL(doge_possess), (gchar*)array);
    }

    wallet_value -= volume;
    printf("new wallet amount is %f\n", wallet_value);
    sprintf(val_txt, "%f", wallet_value);
    gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);
    printf("DONE\n");
}

// appel de change_crypt_amount a l'achat
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

// appel de change_crypt_amount a la vente
void sell(struct Money *strc, float n)
{
    printf("Selling %f %s...\n", n, strc->symbol);
    pos = -1;
    set_lev(strc);
    change_crypt_amount(strc, n);
    update_stoploss_display();
}

// quand on clique manuellement sur buy
void on_buy_button_clicked()
{
    pos = 1;
    if (amount && atof(amount) <= wallet_value && manual == 1)
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

// quand on clique manuellement sur sell
void on_sell_button_clicked()
{
    pos = -1;
    if (amount && manual == 1)
    {
        float a = 0;

        switch (on_money)
        {
            case 0:
                if (strcmp(amount, "all") == 0)
                    a = btc->usd_possess;
                else 
                    a = atof(amount);
                if (a <= btc->usd_possess)
                {
                    if (btc->usd_possess-a < btc->limit)
                        btc->limit = 0;
                    change_crypt_amount(btc, a);
                }
                break;
            case 1:
                if (strcmp(amount, "all") == 0)
                    a = eth->usd_possess;
                else 
                    a = atof(amount);
                if (a <= eth->usd_possess)
                {
                    if (eth->usd_possess-a < eth->limit)
                        eth->limit = 0;
                    change_crypt_amount(eth, a);
                }
                break;
            case 2:
                if (strcmp(amount, "all") == 0)
                    a = doge->usd_possess;
                else 
                    a = atof(amount);
                if (a <= doge->usd_possess)
                {
                    if (doge->usd_possess-a < doge->limit)
                        doge->limit = 0;
                    change_crypt_amount(doge, a);
                }
                break;
            default:
                break;
        }
        update_stoploss_display();
    }
}

void on_value_entry();

// update le nb de monnaie possede
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
    
            if (btc->limit > 0 && btc->priceUsd < btc->limit)
            {
                printf("STOPLOSS ACTION\n");
                btc->limit = 0;
                update_stoploss_display();
  
                pos = -1;
                change_crypt_amount(btc, btc->usd_possess);
            }
            sprintf(array, "%f : %0.3f$", btc->nb_possess, btc->usd_possess);
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
    
            if (eth->limit > 0 && eth->priceUsd < eth->limit)
            {
                printf("STOPLOSS ACTION");
                eth->limit = 0;
                update_stoploss_display();
  
                pos = -1;
                change_crypt_amount(eth, eth->usd_possess);
            }
            sprintf(array, "%f : %0.3f$", eth->nb_possess, eth->usd_possess);
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
    
            if (doge->limit > 0 && doge->priceUsd < doge->limit)
            {
                printf("STOPLOSS ACTION");
                doge->limit = 0;
                update_stoploss_display();
  
                pos = -1;
                change_crypt_amount(doge, doge->usd_possess);
            }
            sprintf(array, "%f : %0.3f$", doge->nb_possess, doge->usd_possess);
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
    if (manual == 0)
    {
        printf("\nbot EVALUATION\n");
        average_crossover(btc);
        average_crossover(eth);
        average_crossover(doge);
    }
}

// thread qui appelle la fonction loop
void begin_loop()
{
    loop();
}

// fonction primaire ouvre l'interface et lance les bails
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

  btc_sl = GTK_WIDGET(gtk_builder_get_object(builder, "btc_sl"));
  eth_sl = GTK_WIDGET(gtk_builder_get_object(builder, "eth_sl"));
  doge_sl = GTK_WIDGET(gtk_builder_get_object(builder, "doge_sl"));

  
  bot_button = GTK_WIDGET(gtk_builder_get_object(builder,"bot_button"));
  
  value_entry = GTK_WIDGET(gtk_builder_get_object(builder,"value_entry"));

  btc_possess = GTK_WIDGET(gtk_builder_get_object(builder,"btc_possess"));

  eth_possess= GTK_WIDGET(gtk_builder_get_object(builder,"eth_possess"));

  doge_possess= GTK_WIDGET(gtk_builder_get_object(builder,"doge_possess"));

  value_label= GTK_WIDGET(gtk_builder_get_object(builder,"value_label"));
  ch24= GTK_WIDGET(gtk_builder_get_object(builder,"ch24"));

  pthread_t thread;
  int  iret;
  iret = pthread_create(&thread, NULL, (void*)begin_loop, NULL);
  if (iret != 0)
    errx(EXIT_FAILURE, "ca bug mais c'est les thread donc c'est chelou");
  sleep(5);
  
  //show exactly the value of the BTC

  char array[100];
  sprintf(array, "%0.2f", btc->priceUsd);
  gtk_label_set_text(GTK_LABEL(value_label), (gchar*)array);
  char array2[100];
  sprintf(array2, "Evolution per 24H = %0.3f%% ",btc->changePercent24Hr);
  gtk_label_set_text(GTK_LABEL(ch24), (gchar*)array2);


  total_money_label= GTK_WIDGET(gtk_builder_get_object(builder,"total_money_label"));

  //we convert wallet_value  in char* to put in a label
  sprintf(val_txt, "%f", wallet_value);
  gtk_label_set_text(GTK_LABEL(total_money_label), (gchar*)val_txt);

 
  //Can't touch
  gtk_widget_show(window);

  gtk_main();
  
  return EXIT_SUCCESS;
}
