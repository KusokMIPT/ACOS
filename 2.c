#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

typedef unsigned int uint;

uint lines_nums = 0, word_num = 0, symb_num = 0;
char* CurfileName = "";

void smartFileReading(FILE **f) {
    *f = fopen(CurfileName, "r");
    if(*f == NULL) {
        perror("Bad File Name");
    }
}

void OneFileProcessoring(FILE* file) {
    char str[10000];
    int flag = 0;

    struct stat spec_struct;
    while((fgets(str, sizeof(str), file)) != NULL) {
        for(int i = 0; i < strlen(str); ++i) {
            if(isspace(str[i]) || flag == 2) {
                flag = 0;
                if(str[i] == '\n') {
                    ++lines_nums;
                }
                continue;
            }

            ++word_num;
            flag = 1;
            while(i + 1 < strlen(str) && !isspace(str[i + 1])) {
                ++i;
            }
        }
        if(flag == 1){
            flag = 2;
        }
        else{
            flag = 0;
        }

        int a = 1;
    }

    //умное определение количество символов
    if (fstat(fileno(file), &spec_struct) != 0) {
        exit(EXIT_FAILURE);
    } else {
        symb_num = spec_struct.st_size;
    }
    printf("%d %d %d", lines_nums, word_num, symb_num);
    lines_nums = 0, word_num = 0, symb_num = 0;
    printf(" %s\n", CurfileName);
}

int main(int argc, char** argv) {
    argc = 2;
    argv[1] = "input.txt";
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            FILE * file;
            CurfileName = argv[i];
            smartFileReading(&file);
            OneFileProcessoring(file);
            fclose(file);
        }
    }
    else {
        FILE* file = stdin;
        OneFileProcessoring(file);
    }

    return 0;
}
