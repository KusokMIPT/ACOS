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

    size_t full_str_capcity = 1000000, full_str_size = 0;//100000
    char* full_str = malloc(full_str_capcity);


    size_t str_capacity = 100000, str_size = 0;
    char* str = malloc(str_capacity);

    //fast string reading
    while((str_size = fread(str, sizeof(char), str_capacity, file)) != 0) {
        if(str_size + 2 + full_str_size > full_str_capcity) {
            full_str_capcity = str_size + full_str_size + 2;
            full_str = realloc(full_str, full_str_capcity);
        }
        memcpy(full_str + full_str_size, str, str_size);
        full_str_size += str_size;
    }

    //processing
    int count = 0;
    for(int i = 0; i < full_str_size; ++i) {
        if(isGoodSymb(full_str[i])) {
            ++count;
        }
        else {
            if(count > 3) {
                n_pos[n_pos_size].beg = i - count;
                n_pos[n_pos_size++].end = i;
            }
            count = 0;
        }
    }

    if(count) {
        n_pos[n_pos_size].beg = full_str_size - count;
        n_pos[n_pos_size++].end = full_str_size;
    }

    for(int i = 0; i < n_pos_size; ++i) {
        Smart_out_string(full_str, n_pos[i].beg, n_pos[i].end);
    }

}

int main(int argc, char** argv) {

    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            FILE * file;
            if(smartFileReading(&file, argv[i])) {
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
