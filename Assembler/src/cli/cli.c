#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cli.h"

void parse_arguments(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "o:v")) != -1)
    {
        switch (opt)
        {
        case 'o':
            printf("Output file: %s\n", optarg);
            break;
        case 'v':
            printf("Assembler version 1.0\n");
            exit(0);
        default:
            fprintf(stderr, "Usage: %s [-o outputfile] [-v]\n", argv[0]);
            exit(1);
        }
    }
}
