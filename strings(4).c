#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

typedef unsigned int uint;

int cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int smartFileReading(FILE **f, char* file_name) {
    *f = fopen(file_name, "r");
    if(*f == NULL) {
        perror("Bad File Name");
        //exit(7);
        return 0;
    }
    return 1;
}

int max(int a, int b) {
    return a > b ? a: b;
}

int isGoodSymb(int c) {
    if((c < 32 && c != '\t') || c > 126) {
        return 0;
    }
    return 1;
}

struct good_sequense {
    int beg;
    int end;
};

void Smart_out_string(char * str, size_t beg, size_t end) {
    for(int i = beg; i < end; ++i) {
        printf("%c", str[i]);
    }
    printf("\n");
}

void strings(FILE* file) {
    size_t n_pos_capacity = 100000, n_pos_size = 0;
    struct good_sequense *n_pos = malloc(n_pos_capacity);

    size_t str_capacity = 10000, str_size = 0;
    char* str = malloc(str_capacity);

    int count = 0;
    int i = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if(str_size + 2 > str_capacity) {
            str_capacity *= 2;
            str = realloc(str, str_capacity);
        }
        str[str_size++] = c;

        if(isGoodSymb(c)) {
            ++count;
        }
        else {
            if(count > 3) {
                n_pos[n_pos_size].end = i;
                n_pos[n_pos_size++].beg = i - count;
            }
            count = 0;
        }

        ++i;
    }
    str[str_size] = '\0';

    for(int i = 0; i < n_pos_size; ++i) {
        Smart_out_string(str, n_pos[i].beg, n_pos[i].end);
    }

}

int main(int argc, char** argv) {
    //argc = 2;
    //argv[1] = "input.txt";
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            FILE * file;
            if(smartFileReading(&file, argv[i])){
                strings(file);
                fclose(file);
            }
        }
    }
    else {
        strings(stdin);
    }

    return 0;
}
