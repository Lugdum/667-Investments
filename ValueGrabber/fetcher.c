#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
int get(void)
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



struct Money {
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



int main(int argc, char** argv) {
    GError *error = NULL;

    if (argc < 2) {
        printf("Usage: test <filename.json>\n");
        return EXIT_FAILURE;
    }

    // Parse the JSON from the file
    JsonParser *parser = json_parser_new();
    json_parser_load_from_file(parser, argv[1], &error);
    if(error) {
        printf("Unable to parse `%s': %s\n", argv[1], error->message);
        g_error_free(error);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }

    // Get the root
    JsonNode *root = json_parser_get_root(parser);

    // Turn the root into a JSON object
    JsonObject *stuff = json_node_get_object(root);

    // Get each object member and assign it to the struct.
    struct Books book = {
        .set = json_object_get_int_member(stuff, "set"),
        .low = json_object_get_int_member(stuff, "low"),
        // Duplicate the strings to avoid pointing to memory inside the parser.
        .draw_set = g_strdup(json_object_get_string_member(stuff, "draw_set")),
        .wupet = g_strdup(json_object_get_string_member(stuff, "Wupet"))
    };

    printf(
        "id = %ld, rank= %ld, symbol = '%s', name = '%s', supply = %ld, maxSupply = %ld, marketCapUsd = '%s', volumeUsd24 = '%s', priceUsd = %ld, changePercent24Hr = '%s', vwap24Hr = '%s'\n",
        book.set, book.low, book.draw_set, book.wupet
    );

    // We're finished working with the parser. Deallocate the
    // parser and all the memory it has allocated, including
    // the nodes.
    g_object_unref(parser);

    return EXIT_SUCCESS;
}
