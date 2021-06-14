#ifndef UI_H
#define UI_H

int* get_histo(struct Money *money);
float wallet_value;
void buy(struct Money *money, float volume);
void sell(struct Money *money, float volume);
int open_interface();
extern int on_money;
void update_image();

#endif
