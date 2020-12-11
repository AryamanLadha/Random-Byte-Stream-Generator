//Process the input options
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "./options.h"

void read_options(
    int argc,
    char* argv[],
    struct opts* opts
){
    opts->valid = true;
    int c;
    while ((c = getopt(argc, argv, ":i:o:")) != -1) {
        switch(c) {
        case 'i':
            if (strcmp("rdrand", optarg) == 0){
            opts->input = RDRAND;
            }
            else if (strcmp("mrand48_r", optarg) == 0){
            opts->input = MRAND48_R;
            }
            else if ('/' == optarg[0]){
            opts->input = SLASH_F;
            opts->r_src = optarg;
            // opts->r_src = (char*)malloc(strlen(optarg)+1);
            // strcpy(opts->r_src, optarg);
            }
            else{
            opts->valid = false;
            break;
            }
            break;
        case 'o':
            if (strcmp("stdio", optarg) == 0){
            opts->output = STDOUT;
            }
            else if((opts->block_size = atoi(optarg))){
            opts->output = N;
            //opts->block_size = atoi(optarg);
            }
            else{
            opts->valid = false;
            break;
            }
            break;
        case ':': 
            break;
        case '?':
            break;
        }
    }
    

    if (optind >= argc) {
        return;
    }

    opts->nbytes = atol(argv[optind]);
    if(opts->nbytes >= 0){
    opts->valid = true;
    }
}
