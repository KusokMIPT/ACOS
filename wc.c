#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

typedef unsigned int uint;

uint lines_nums = 0, word_num = 0, symb_num = 0;
char* CurfileName = "";

int smartFileReading(FILE **f) {
    *f = fopen(CurfileName, "r");
    if(*f == NULL) {
        perror("Bad File Name");
        return 0;
    }
    return 1;
}

void OneFileProcessoring(FILE* file) {
    int flag = 0;

    int c;
    while((c = fgetc(file)) != EOF){
        ++symb_num;
        if(isspace(c)){
            if(c == '\n'){
                ++lines_nums;
            }
            if(flag) {
            ++word_num;
            }
            flag = 0;

            continue;
        }

        flag = 1;
    }
    if(flag){
        word_num++;
    }
    printf("%d %d %d %s\n", lines_nums, word_num, symb_num, CurfileName);
    lines_nums = 0, word_num = 0, symb_num = 0;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            FILE * file;
            CurfileName = argv[i];
            if(smartFileReading(&file)){
                OneFileProcessoring(file);
                fclose(file);
            }
        }
    }
    else {
        OneFileProcessoring(stdin);
    }

    return 0;
}
