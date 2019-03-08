#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

typedef unsigned int uint;

void smartFileReading(FILE **f, char* file_name) {
    *f = fopen(file_name, "r");
    if(*f == NULL) {
        perror("Bad File Name");
        //exit(7);
    }
}

int max(int a, int b) {
    return a > b ? a: b;
}

void out_n_pos(int* arr, int size) {
    for(int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
}

void OutFileTail(FILE* file) {
    size_t len = 500000;
    ssize_t str_size = 0;
    char* str = malloc(len);

    size_t lines_size = 0, lines_capcity = 10000;
    char **lines = malloc(sizeof(char*) * lines_capcity);

    while((str_size = getline(&str, &len, file)) != -1){
            if(lines_size + 2  > lines_capcity){
                lines_capcity*=2;
                lines = realloc(lines, sizeof(char*)*lines_capcity);
            }

            lines[lines_size] = malloc(str_size + 3);
            strcpy(lines[lines_size++], str);
    }

    for(int i = max(0, lines_size - 10); i < lines_size; ++i){
        printf("%s", lines[i]);
    }

    free(lines);
    free(str);
}

int main(int argc, char** argv) {
    //argc = 2;
    //argv[1] = "input.txt";
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
        OutFileTail(stdin);
    }

    return 0;
}
