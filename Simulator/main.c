#include "../ValueGrabber/fetcher.h"
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>

void add_money(long * wallet, long x)
{
    *wallet += x;
    if (x >= 0)
        printf("+");
    printf("%ld$\n", x);
}

// Buy for 'amount_eur' EUR at a price of 'price_cpt' from the account sold 'wallet'
// Return the ammount of crypto bought
long buy(long amount_eur, long price_cpt, long * wallet)
{
    if (amount_eur <= 0)
        errx(EXIT_FAILURE, "You cannot buy a negative amount of crypto\n");
    else if (amount_eur > *wallet)
        errx(EXIT_FAILURE, "You do not have enouhg money\n");
    
    add_money(wallet, -amount_eur);


    return amount_eur/price_cpt;
}

//int open_placement()

int main(int argc, char** argv)
{
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: ./main {sold} {crypto}");

    unsigned long *wallet;
    *wallet = atol(argv[1]);
    if (*wallet <= 0)
        errx(EXIT_FAILURE, "Invalid argument '%s', you must enter a positive amount.", argv[1]);
    
    char **cpt_list = malloc((argc-2)*sizeof(char*));
    
    for (int i = 2; i < argc; i++)
        *(cpt_list+i-2) = argv[i];
    
    get_strc_list(argc-2, cpt_list);

    long * bought_money_list = calloc((argc-2), sizeof(long));
    

    return 0;
}
