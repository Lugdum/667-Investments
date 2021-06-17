#ifndef BOT_H
#define BOT_H

void write_history(struct Money *money);
int* get_history(struct Money *money);
void average_crossover(struct Money *money);
extern int hist_len;
float volume;

#endif
