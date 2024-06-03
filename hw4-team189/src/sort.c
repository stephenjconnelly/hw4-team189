#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

void display_error(int num, char* arg) {


    switch (num) {
        case 1:
            fprintf(stderr, "Error: Unknown option '-%s' received.\n"
"Usage: ./sort [-i|-d] [filename]\n"
"   -i: Specifies the input contains ints.\n"
"   -d: Specifies the input contains doubles.\n"
"   filename: The file to sort. If no file is supplied, input is read from\n"
"             stdin.\n"
"   No flags defaults to sorting strings.\n", arg);
            break;
        case 2:
            fprintf(stderr, "Error: Too many files specified.\n");
            break;
        case 3:
            fprintf(stderr, "Error: Too many flags specified.\n");
            break;
    }
}

int main(int argc, char **argv) {
    int iflag = 0;
    int dflag = 0;
    char *fname = NULL;
    int max_parameters = 2;
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "id")) != -1) {
        switch (c)
            {
                case 'i':
                    iflag = 1;
                    break;
                case 'd':
                    dflag = 1;
                    break;
                case '?': ;
                    char bad_flag;
                    bad_flag = optopt;
                    display_error(1, &bad_flag);
                    return EXIT_FAILURE;
            }
    }
    if (iflag && dflag) {
        //Too many flags specified.
        display_error(3, 0);
        return EXIT_FAILURE;
    } else if (iflag || dflag) {
        max_parameters = 3;
    }

    if (argc > max_parameters) {
        //too many files
        display_error(2, 0);
        return EXIT_FAILURE;
    }
    fname = argv[optind];
    if (fname != NULL) {
        //pipe the contents of some file into stdin
        FILE *fp = freopen(fname, "r", stdin);
        if (fp == NULL) {
            fprintf(stderr, "Error: Cannot open source file '%s'. %s\n", fname, strerror(errno));
            return EXIT_FAILURE;
        }
    }
    if (iflag) {
        char input[MAX_STRLEN+1];
        int nums[MAX_ELEMENTS];
        int i = 0;
        while (fgets(input, sizeof(input), stdin) != NULL && i < MAX_ELEMENTS) {
            input[strcspn(input, "\n")] = '\0';
            nums[i] = atoi(input);
            i++;
        }

        quicksort(nums, i, sizeof(int), int_cmp);
        for (int j = 0; j < i; j++) {
            printf("%d\n", nums[j]);
        }

    } else if (dflag) {
        char input[MAX_STRLEN+1];
        double nums[MAX_ELEMENTS];
        int i = 0;
        while (fgets(input, sizeof(input), stdin) != NULL && i < MAX_ELEMENTS) {
            input[strcspn(input, "\n")] = '\0';
            nums[i] = atof(input);
            i++;
        }

        quicksort(nums, i, sizeof(double), dbl_cmp);
        for (int j = 0; j < i; j++) {
            printf("%lf\n", nums[j]);
        }


    } else {
        char input[MAX_STRLEN+1];
        char *lines[MAX_ELEMENTS];

        int i = 0;

        while (fgets(input, sizeof(input), stdin) != NULL
                && i < MAX_ELEMENTS) {
                    input[strcspn(input, "\n")] = '\0';
                    lines[i] = malloc(strlen(input) + 1);
                    if (lines[i] == NULL) {
                        for (int j = 0; j < i; j++) {
                            free(lines[j]);
                        }
                        fprintf(stderr, "Memory error.");
                        return EXIT_FAILURE;
                    }
                    strcpy(lines[i], input);
                    i++;
        }
        quicksort(lines, i, 8, str_cmp);
        for (int j = 0; j < i; j++) {
            printf("%s\n", lines[j]);
        }
        for (int j = 0; j < i; j++) {
            free(lines[j]);
        }


    }
    return EXIT_SUCCESS;
}