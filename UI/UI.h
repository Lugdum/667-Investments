#ifndef UI_H
#define UI_H

int* get_histo(struct Money *money);
void on_btc_graph_button_toggled();
void on_eth_graph_button_toggled();
void on_yfi_graph_button_toggled();
void update_possess_money_price();
float wallet_value;
void buy(struct Money *money, float volume);
void sell(struct Money *money, float volume);
int open_interface();
extern int on_money;
void update_image();

#endif
