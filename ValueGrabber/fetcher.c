#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

int main(void)
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

/*

struct Money
{
  gint64      id;
  gint64      rank;
  char        *symbol;
  char        *name;
  gint64      supply;
  gint64      maxSupply;
  gint64      marketCapUsd;
  gint64      volumeUsd24Hr;
  gint64      priceUsd;
  gint64      changePercent24Hr;
  gint64      vwap24Hr;
};



int main()
{
  // Get each object member and assign it to the struct.
  struct Money money =
    {
     .id = json_object_get_int_member(stuff, "id"),
     .rank = json_object_get_int_member(stuff, "rank"),
     // Duplicate the strings to avoid pointing to memory inside the parser.
     .symbol = g_strdup(json_object_get_string_member(stuff, "symbol")),
     .name = g_strdup(json_object_get_string_member(stuff, "name")),
     .supply = json_object_get_int_member(stuff, "supply"),
     .maxSupply = json_object_get_int_member(stuff, "maxSupply"),
     .marketCapUsd = json_object_get_int_member(stuff, "marketCapUsd"),
     .volumeUsd24Hr = json_object_get_int_member(stuff, "volumeUsd24Hr"),
     .priceUsd = json_object_get_int_member(stuff, "priceUsd"),
     .changePercent24Hr = json_object_get_int_member(stuff, "changePercent24Hr"),
     .vwap24Hr = json_object_get_int_member(stuff, "vwap24Hr")
    };

  printf(
	 "id = %ld, rank= %ld, symbol = '%s', name = '%s', supply = %ld, maxSupply = %ld, marketCapUsd = '%ld', volumeUsd24Hr = '%ld', priceUsd = %ld, changePercent24Hr = '%ld', vwap24Hr = '%ld'\n",
	 money.id, money.rank, money.symbol, money.name, money.supply, money.maxSupply, money.marketCapUsd, money.volumeUsd24Hr, money.priceUsd, money.changePercent24Hr, money.vwap24Hr
	 );

  return EXIT_SUCCESS;
}


int Search_and_replace(char *file, char *c)
{
  int index;

  c = strchr(file, c);
  index = (int)(c - file);
  *(file+index) = ".";
  return index;
}


int Search_in_File(char *fname, char *str)
{
  char * buffer = 0;
  long length;
  FILE * f = fopen (filename, "rb");

  if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = malloc (length);
      if (buffer)
	{
	  fread (buffer, 1, length, f);
	}
      fclose (f);
    }

  if (buffer)
    {
      Search_and_replace(*buffer, "\"");
      Search_and_replace(*buffer, "\"");
      for (int i = 11; i > 0; i--)
	{
	  Search_and_replace(*buffer, "\"");
	  Search_and_replace(*buffer, "\"");
	  int first = Search_and_replace(*buffer, "\"")+1;
	  int sec = Search_and_replace(*buffer, "\"");
	}
    }
  FILE *fp;
  int line_num = 1;
  int find_result = 0;
  char temp[512];
	
  gcc users
    if((fp = fopen(fname, "r")) == NULL) {
      return(-1);
    }

  while(fgets(temp, 512, fp) != NULL) {
    if((strstr(temp, str)) != NULL) {
      printf("A match found on line: %d\n", line_num);
      printf("\n%s\n", temp);
      find_result++;
      if (find_result == 5)
	
			
    }
    line_num++;
  }

  if(find_result == 0) {
    printf("\nSorry, couldn't find a match.\n");
  }
	
  //Close the file if still open.
  if(fp) {
    fclose(fp);
  }
  return(0);
}
*/
