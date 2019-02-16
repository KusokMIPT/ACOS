#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
void split(char * str, size_t size, char* res){

	res = realloc(res,1);
	unsigned int res_size = 1;

	for(int i = 0 ; i < size; ++i){
	if(isspace(str[i])){
	continue;
	}

	res[res_size++] = str[i];
	}

	printf("%d", res_size);
	return;
}
*/

int compare_string (const void * str1, const void * str2) {
    const char *s1 = *(const char**)str1;
    const char *s2 = *(const char**)str2;

    return strcmp(s1, s2);
}

int max(int a, int b) {
    return a > b?a:b;
}

void out_str_array(char **str, int size) {
    for(int i = 0; i < size; ++i) {
        if (i!= size -1)printf("%s\n", str[i]);

        else printf("%s", str[i]);
    }
}

int isLeksema(char c) {
    if(c == ';' || c == '&' || c == '&&' || c =='|' || c =='||')
        return 1;
    return 0;
}

int main() {
    freopen("input.txt","r", stdin);

    char c;
    char *str = malloc(1);

    unsigned int size = 0;

    ///String's reading

    while ((c = getchar()) != EOF) {
        str[size++] = c;
        str = realloc(str, size+1);
    }
    str[size] = '\0';

    ///Processing of String's

    char **strs = NULL;
    strs = malloc(sizeof(char *));

    strs[0] = malloc(sizeof(char));
    unsigned int cur_size = 1, num_of_str = 0;

    int i = 0;
    while(isspace(str[i])) {
        ++i;
    }

    int flag1 = 0, flag2 = 0;
    for(; i < size; ++i) {

        if(isspace(str[i])) {

            int zashel = 0;
            // ++i; ТОЧНО НЕ НАПРАВИЛЬНО!
            while(isspace(str[i]) && i < size) {
                ++i;
                zashel = 1;
            }
            if(flag1) continue;

            strs[num_of_str] = realloc(strs[num_of_str], cur_size + 2);
            strs[num_of_str][cur_size] = '\"';
            strs[num_of_str][cur_size + 1] = '\0';

            //Check str Ending
            if(i == size) break;

            strs = realloc(strs, (max(num_of_str + 1,num_of_str * 2))*sizeof(char *));

            ++num_of_str;
            cur_size = 1;
            if(zashel) --i;
            strs[num_of_str] = malloc(sizeof(char));
            flag2 = 1;

            continue;
        }

        if(isLeksema(str[i])) {
            if(i > 0 && !isspace(str[i-1])) {
                strs[num_of_str][cur_size] = '\"';
                strs[num_of_str][cur_size+1] = '\0';
            }
            flag1 = 1;
            //Случай когда это первый встреченный символ
            if(cur_size != 1) {
                strs = realloc(strs, max(++num_of_str + 1, num_of_str * 2) * sizeof(char *));//Сразу 2 аллоцируем
            }
            strs[num_of_str] = malloc(sizeof(char) * 5);
            strs[num_of_str][0] = '\"';
            strs[num_of_str][1] = str[i];
            if(i+1 < size && str[i+1] == str[i]) {

                strs[num_of_str][2] = str[i];

                strs[num_of_str][3] = '\"';
                strs[num_of_str][4] = '\0';

                num_of_str++;
                strs[num_of_str] = malloc(sizeof(char));

                cur_size = 1;
                ++i;
                continue;
            }
            strs[num_of_str][2] = '\"';
            strs[num_of_str][3] = '\0';

            num_of_str++;
            if(i+1 < size && !isspace(str[i+1]))
                strs[num_of_str] = malloc(sizeof(char));

            cur_size = 1;
            continue;
        }

        /// If not Lekseme and no space4
        flag1 = 0;
        strs[num_of_str] = realloc(strs[num_of_str], max(cur_size + 1,cur_size*2 ));
        strs[num_of_str][0] = '\"';

        strs[num_of_str][cur_size] =  str[i];
        ++cur_size;
    }


    if(num_of_str != 0 && flag2 && !flag1) ++num_of_str;
    else {
        if(cur_size != 1) ++num_of_str;
    }


    ///Sort list of string's
    qsort(strs, num_of_str, sizeof(char *), compare_string);

    out_str_array(strs, num_of_str);


    free(str);

    for(int i = 0; i < num_of_str; ++i) {
        free(strs[i]);
    }

    free(strs);

    return 0;
}
