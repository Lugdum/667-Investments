#ifndef FETCHER_H
#define FETCHER_H

struct Money **get_strc_list(int argc, char** argv);
struct Money *get_strc(char* name);
void printstruct(struct Money *money);

#endif
