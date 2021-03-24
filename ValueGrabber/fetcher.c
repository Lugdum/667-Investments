#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>


/* read the api from "api.coincap.io" and write it into "output.txt" */
void update_value(char *m_type)
{

    CURL *curl;
    CURLcode res;
    char url[] = "api.coincap.io/v2/assets/";
    strcat(url, m_type);
    printf("Downloading your data at adress: %s\n", url);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* create an output file and prepare to write the response */
        FILE *output_file = fopen("output.txt", "w");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* errors handle */
        if(res != CURLE_OK) {
            errx(EXIT_FAILURE, "curl_easy_perform() failed: %sn", curl_easy_strerror(res));
        }

        /* cleanup */
        curl_easy_cleanup(curl);
	fclose(output_file);
    }
}

/* money structure declaration */
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

/* print the struct money */
void printstruct(struct Money *money)
{
    printf("----------------------\nid = %s\nrank = %d\nsymbol = '%s'\nname = '%s'\nsupply = %f\nmaxSupply = %f\nmarketCapUsd = '%f'\nvolumeUsd24Hr = '%f'\npriceUsd = %f\nchangePercent24Hr = '%f'\nvwap24Hr = '%f'\n----------------------\n",
	 money->id, money->rank, money->symbol, money  ->name, money->supply, money->maxSupply, money->marketCapUsd, money->volumeUsd24Hr, money->priceUsd, money->changePercent24Hr, money->vwap24Hr);
    /*printf("id = %s, rank = %d, symbol = '%s', name = '%s', supply = %f, maxSupply = %f, marketCapUsd = '%f', volumeUsd24Hr = '%f', priceUsd = %f, changePercent24Hr = '%f', vwap24Hr = '%f'\n",
	 money.id, money.rank, money.symbol, money.name, money.supply, money.maxSupply, money.marketCapUsd, money.volumeUsd24Hr, money.priceUsd, money.changePercent24Hr, money.vwap24Hr);*/
}

/* copy a string and return it */
char *strcopy(char *tmp, size_t len)
{
    char *str = calloc(len, sizeof(char));
    for (int i = 0; i < len ; i++) *(str+i) = *(tmp+i);

    return str;
}



struct Money *getmoney(char *buf)
{
    int pick_data = 0;
    int i_tmp = 0;
    int a = 0;
    struct Money *money = malloc(sizeof(struct Money));
    char *tmp = malloc(1*sizeof(char));
    size_t len = 0;

    //printf("Creating struct money...\n");

    for (int i = 1; a < 11; i++)
    {
        //printf("%d : char is %c\n", i, *(buf+i)+'\0');
        if (pick_data == 0 && *(buf+i-1) == ':'  && *(buf+i) == '"')
        {
            //printf("Relevant data found at index %d\n", i);
            pick_data++;
            i_tmp = 0;
            len = 0;
            //printf("Creating temporary buffer for the data.\n");
            while (*(buf+i+len+1) != '"') len++;
            tmp = realloc(tmp, (len+1)*sizeof(char));
            //printf("Buffer of size %ld created.\n", len+1);
        }
        else if (pick_data == 0 && *(buf+i-1) == ':'  && *(buf+i) == 'n')
        {
            a++;    
        }

        else if (pick_data == 1 && *(buf+i) == '"')
        {
            //printf("End of data, storing it in struct.\n");
            pick_data = 0;
            *(tmp+i_tmp) = '\0';
            switch(a)
            {
                case 0:
                    money->id = strcopy(tmp, len+1);
                    //printf("id is now %s\n", tmp);
                    break;
                case 1:
                    money->rank = atof(tmp);
                    break;
                case 2:
                    money->symbol = strcopy(tmp, len+1);
                    //printf("symbol is now %s\n",tmp);
                    break;
                case 3:
                    money->name = strcopy(tmp, len+1);
                    //printf("name is now %s\n", tmp);
                    break;
                case 4:
                    money->supply = atof(tmp);
                    break;
                case 5:
                    money->maxSupply = atof(tmp);
                    break;
                case 6:
                    money->marketCapUsd = atof(tmp);
                    break;
                case 7:
                    money->volumeUsd24Hr = atof(tmp);
                    break;
                case 8:
                    money->priceUsd = atof(tmp);
                    break;
                case 9:
                    money->changePercent24Hr = atof(tmp);
                    break;
                case 10:
                    money->vwap24Hr = atof(tmp);
                    break;
                default:
                    break;
            }
	    a++;
        }
        else if (pick_data == 1)
        {
            //printf("Adding char %c in the temporary buffer.\n", *(buf+i));
            *(tmp+i_tmp) = *(buf+i);
            i_tmp++;
        }
    }

    //printf("Struct money created.\n");
    //printf("END OF FOR\n");

    printf("Struct updated\n");

    return money;
}


/* get data in file and write it into buffer */
struct Money *Search_in_File(char *fname)
{
    struct Money *money;
    size_t length;
    FILE *f = fopen (fname, "r");

    //printf("%s is open\n", fname);

    if (f)
    {
        fseek(f, 0L, SEEK_END);
        length = ftell(f);
        //printf("length is %ld\n", length);
        fseek (f, 0, SEEK_SET);
        char *buffer = calloc(length, sizeof(char));
        if (buffer)
	    {
	        if (fread(buffer, length, 1, f) == -1)
            err(1,"Coufn't read file %s\n", fname);
		      fclose (f);

		      money = getmoney(buffer);
	    }
    }
    return money;
}

int main(int argc, char** argv)
{
    printf("srdgfsdfsd");
  // Update value in file
  update_value(argv[1]);

  // Get each object member and assign it to the struct.
  struct Money *money = Search_in_File("output.txt");

  // Print the struct money
  printstruct(money);

  return EXIT_SUCCESS;
}
