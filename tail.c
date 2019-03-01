#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

typedef unsigned int uint;

int cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

void smartFileReading(FILE **f, char* file_name) {
    *f = fopen(file_name, "r");
    if(*f == NULL) {
        perror("Bad File Name");
        exit(7);
    }
}

int max(int a, int b) {
    return a > b ? a: b;
}

void out_string(char * str, size_t beg, size_t end) {
    int i;
    if(beg == 0) i = 0;
    else i = beg + 1;

    for(; i < end; ++i) {
        printf("%c", str[i]);
    }
    if(str[beg + 1] != '\n') printf("\n");
}

void out_n_pos(int* arr, int size) {
    for(int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
}

void OutFileTail(FILE* file) {
    fseek(file, 0L, SEEK_END);//переход в конец файла
    size_t str_capacity = 1000000, str_size = 0;
    char* str = malloc(str_capacity);
    size_t file_len = ftell(file); //For what?

    size_t n_pos_capacity = 100, n_pos_size = 1;
    int *n_pos = malloc(n_pos_capacity);
    n_pos[0] = 0;

    fseek(file, 0L, SEEK_SET);

    int i = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if(i + 2 > str_capacity) {
            str_capacity*=2;
            str = realloc(str, str_capacity);
        }
        str[str_size++] = c;

        if(c == '\n') {
            if(n_pos_size +  2 > n_pos_capacity) {
                n_pos_capacity*=2;
                n_pos = realloc(n_pos, n_pos_capacity);
            }
            n_pos[n_pos_size++] = i;
        }
        ++i;
    }
    if(str[str_size - 1] != '\n') {
        n_pos_size = (n_pos_size + 1) % 10;
        n_pos[n_pos_size] = str_size - 1;
    }

    qsort(n_pos, n_pos_size, sizeof(int), cmp);
    //out_n_pos(n_pos, n_pos_size);
    str[str_size] = '\0';

    for(int i = max(n_pos_size - 11, 0); i < n_pos_size; ++i) {
        out_string(str, n_pos[i], n_pos[i+1]);
    }
}

int main(int argc, char** argv) {
    argc = 2;
    argv[1] = "input.txt";
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            FILE * file;
            smartFileReading(&file, argv[i]);
            printf("==> %s <==\n", argv[i]);
            OutFileTail(file);

            fclose(file);
        }
    }
    else {
        FILE* file = stdin;

    }

    return 0;
}
