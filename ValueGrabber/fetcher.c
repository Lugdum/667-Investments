#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>



int update_value(void)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "api.coincap.io/v2/assets/bitcoin");

        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* create an output file and prepare to write the response */
        FILE *output_file = fopen("output.txt", "w");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %sn", 
                curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
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



char *strcopy(char *tmp, size_t len)
{
    char *str = calloc(len, sizeof(char));
    for (int i = 0; i < len ; i++) *(str+i) = *(tmp+i);

    return str;
}



struct Money getmoney(char *buf)
{
    int pick_data = 0;
    int i_tmp = 0;
    int a = 0;
    struct Money money;
    char tmp[1]; 
    size_t len = 0;

    printf("Creating struct money...\n");

    for (int i = 1; a < 11; i++)
    {  
        printf("%d : char is %c\n", i, *(buf+i)+'\0');
        if (pick_data == 0 && *(buf+i-1) == ':' && *(buf+i) == '"')
        {
            printf("Relevant data found at index %d\n", i);
            pick_data++;
            i_tmp = 0;
            len = 0;
            printf("Creating temporary buffer for the data.\n");
            while (*(buf+i+len+1) != '"') len++;
            char tmp[len+1];
            printf("Buffer of size %ld created.\n", len+1);
        }
        else if (pick_data == 1 && *(buf+i) == '"')
        {
            printf("End of data, storing it in struct.\n");
            pick_data--;
            *(tmp+i_tmp) = '\0';
            switch(a)
            {
                case 0:
                    money.id = strcopy(tmp, len+1);
                    printf("id is now %s\n", tmp);
                    break;
                case 1:
                    money.rank = atof(tmp);
                    break;
                case 2:
                    money.symbol = strcopy(tmp, len+1);
                    printf("symbol is now %s\n",tmp);
                    break;
                case 3:
                    money.name = strcopy(tmp, len+1);
                    printf("name is now %s\n", tmp);
                    break;
                case 4:
                    money.supply = atof(tmp);
                    break;
                case 5:
                    money.maxSupply = atof(tmp);
                    break;
                case 6:
                    money.marketCapUsd = atof(tmp);
                    break;
                case 7:
                    money.volumeUsd24Hr = atof(tmp);
                    break;
                case 8:
                    money.priceUsd = atof(tmp);
                    break;
                case 9:
                    money.changePercent24Hr = atof(tmp);
                    break;
                case 10:
                    money.vwap24Hr = atof(tmp);
                    break;
                default:
                    break;
            }
	    a++;
        }
        else if (pick_data == 1)
        {
            printf("Adding char %c in the temporary buffer.\n", *(buf+i));
            *(tmp+i_tmp) = *(buf+i);
            i_tmp++;
        }
    }
    printf("Struct money created.\n");
   printf(
	 "id = %s, rank= %d, symbol = '%s', name = '%s', supply = %f, maxSupply = %f, marketCapUsd = '%f', volumeUsd24Hr = '%f', priceUsd = %f, changePercent24Hr = '%f', vwap24Hr = '%f'\n",
	 money.id, money.rank, money.symbol, money.name, money.supply, money.maxSupply, money.marketCapUsd, money.volumeUsd24Hr, money.priceUsd, money.changePercent24Hr, money.vwap24Hr
	 );
  printf("END OF FOR\n");

    return money;
}



struct Money Search_in_File(char *fname)
{
    struct Money money;
    size_t length;
    FILE *f = fopen (fname, "r");

    printf("%s is open\n", fname);

    if (f)
    {
        fseek(f, 0L, SEEK_END);
        length = ftell(f);
        printf("length is %ld\n", length);
        fseek (f, 0, SEEK_SET);
        char *buffer = calloc(length, sizeof(char)); 
        if (buffer)
	    {
	        if (fread(buffer, length, 1, f) == -1) err(1,"Coufn't read file %s\n", fname);
		fclose (f);
		
		return getmoney(buffer);
	    }
    }
    return money;
    err(1,"Coufn't create buffer");
}



int main()
{
  // Update value in file
  int u = update_value();
  // Get each object member and assign it to the struct.
  struct Money money = Search_in_File("output.txt");
  printf(
	 "id = %s, rank = %d, symbol = '%s', name = '%s', supply = %f, maxSupply = %f, marketCapUsd = '%f', volumeUsd24Hr = '%f', priceUsd = %f, changePercent24Hr = '%f', vwap24Hr = '%f'\n",
	 money.id, money.rank, money.symbol, money.name, money.supply, money.maxSupply, money.marketCapUsd, money.volumeUsd24Hr, money.priceUsd, money.changePercent24Hr, money.vwap24Hr
	 );
  
  return EXIT_SUCCESS;
}
