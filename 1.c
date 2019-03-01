#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef unsigned int uint;

int compare_string (const void * str1, const void * str2) {
    const char *s1 = *(const char**)str1;
    const char *s2 = *(const char**)str2;

    return strcmp(s1, s2);
    return(strcmp( *(char**)str1, *(char**)str2));
}

char* stradd(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*) malloc(len * sizeof(char) + 10);
    *ret = '\0';
    return strcat(strcat(ret, a) ,b);
}

int max(int a, int b) {
    return a > b ? a:b;
}

void out_str_array(char **str, int size) {
    for(int i = 0; i < size; ++i) {
        printf("\"%s\"\n", str[i]);
    }
}

int isLeksema(char* str, uint cur_pos, uint size) {

    char c = str[cur_pos];

    if(c == ';' || c== '&' || c =='|') {
        return (cur_pos + 1 < size && str[cur_pos + 1] == str[cur_pos] &&
            (str[cur_pos + 1] == '|'|| str[cur_pos + 1] == '&')) ? 2 : 1;
    }

    return 0;
}

int isQuot(char* str, uint cur_pos, uint size) {

    if( str[cur_pos] != '\'' && str[cur_pos] != '\"' )
        return 0;

    int last_pos = 0;
    for(int i = cur_pos + 1; i < size; ++i) {
        if(str[i] == str[cur_pos]) {
            return i;
        }
    }

    perror("Synt error");
    exit(7);
}

///For line string
uint str_capacity = 7;
char *str = NULL;
uint size = 0, symb_num = 0;

///For dict
char **strs = NULL;
uint num_of_str = 0, strs_capacity = 7;

void add_word_to_str();

void_smart_malloc() {

    str = malloc(str_capacity);

    if(str == NULL) {
        perror("Bad alloc");
        exit(7);
    }

    strs = malloc(sizeof(char *) * strs_capacity);

    if(strs == NULL) {
        perror("Bad alloc");
        exit(7);
    }
}

void realloc_and_check(char ** str_to_realloc, uint *capacity) {
    *capacity *= 2;
    *str_to_realloc = realloc(*str_to_realloc, *capacity);
    if (str_to_realloc == NULL) {
        perror("Bad alloc");
        exit(7);
    }
}

int main() {
    void_smart_malloc();
    int c;
    while ((c = getchar()) != EOF) {
        str[size++] = c;

        if(size + 2 >= str_capacity) {
            realloc_and_check(&str, &str_capacity);
        }
    }
    str[size] = '\0';

    string_processoring();

    qsort(strs, num_of_str, sizeof(char *), compare_string);

    out_str_array(strs, num_of_str);

    free(str);
    for(int i = 0; i < num_of_str; ++i) {
        free(strs[i]);
    }
    free(strs);

    return 0;
}

void add_word_to_arr(char* word, uint word_size) {
    if(word_size < 1) return;

    word[word_size] = '\0';

    if(num_of_str + 2 >= strs_capacity) {
        strs_capacity *= 2;
        strs = realloc(strs, sizeof(char *) * (strs_capacity * 2));

        if(strs == NULL) {
            perror("Bad alloc");
            exit(7);
        }
    }

    strs[num_of_str] = malloc(word_size + 1);
    if(strs[num_of_str] == NULL) {
        perror("Bad alloc");
        exit(7);
    }

    strs[num_of_str] = word;
}

void string_processoring() {
    uint i = 0;

    for(; i<size; ++i) {
        uint leks = isLeksema(str, i, size);
        uint quot = isQuot(str, i, size);

        if(isspace(str[i]) || str[i] == '\0') {

            continue;
        }

        if(leks) {

            char *word = malloc(7);
            uint word_size = 0;
            if(leks == 2) {
                word[word_size++] = str[i];
                word[word_size++] = str[i++];
            }
            else {
                word[word_size++] = str[i];
            }

            word[word_size] = '\0';

            add_word_to_arr(word, word_size);
            ++num_of_str;

            word = NULL;
            word_size = 0;
            continue;
        }

        if(quot) {
            char* word = NULL;
            int flag = 1;
            for (int k = i - 1; k>=0; --k) {
                if(!isspace(str[k])) {
                    flag = 0;
                    break;
                }
                if (str[k] == '\n')
                    break;
            }

            if(i == 0 || flag ) {
                word = malloc(7);
            }
            else {
                if(num_of_str > 0 && isspace(str[i-1])) {
                    word = malloc(7);
                }
                else {
                    word = malloc(strlen(strs[num_of_str]) + 1);
                    word = strs[--num_of_str];
                    word[strlen(word)] = 0; // Удаляем лишний '\0'
                }
            }
            uint word_size = 0, word_capacity = 7;
            char *added_to_word = malloc(word_capacity);

            for (int j = i + 1; j <= quot-1; ++j) {
                if(word_size + 2 >= word_capacity) {
                    word_capacity*=2;
                    added_to_word = realloc(added_to_word, word_capacity);
                }

                added_to_word[word_size++] = str[j];
            }
            i = quot;
            for (int j = i+1; j < size &&!isspace(str[i + 1]) && !isLeksema(str, i + 1, size) && !isQuot(str, i+1, size) && str[i] == '\0'; ++j) {
                if(word_size + 2 >= word_capacity) {
                    realloc_and_check(&word, &word_capacity);
                }
                added_to_word[word_size++] = str[j];
                ++i;
            }

            added_to_word = stradd(added_to_word, "\0");
            word = stradd(word, added_to_word);

            add_word_to_arr(word, strlen(word));

            if(strlen(word) != 0) {
                add_word_to_arr(word, word_size);
                ++num_of_str;
            }
            continue;
        }

        uint word_capacity = 7, word_size = 0;
        char * word = malloc(word_capacity);
        word[word_size++] = str[i];


        while (i+1 < size && !isspace(str[i + 1]) && !isLeksema(str, i + 1, size) && !isQuot(str, i+1, size)) {
            if(word_size + 2 >= word_capacity) {
                realloc_and_check(&word, &word_capacity);
            }
            word[word_size++] = str[++i];
        }

        add_word_to_arr(word, word_size);
        ++num_of_str;

        word = NULL;
        word_size = 0;
    }
}
