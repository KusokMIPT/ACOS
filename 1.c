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

void out_str_array(char **str, int size){
    for(int i = 0; i < size; ++i){
    printf("%s\n", str[i]);
    }
}

int main(){
    //freopen("input.txt","r", stdin);
	char c;
	char *str = NULL;
	str = realloc(str,1);
	unsigned int size = 0;

	///String's reading

	while ((c = getchar()) != EOF){
	str[size++] = c;
	str = realloc(str, size+1);
	}
	str[size] = '\0';

	///Processing of String's

	char **strs = NULL;
    strs = realloc(strs, 1);
	unsigned int cur_size = 1, num_of_str = 0;

	for(int i = 0 ; i < size; ++i){

	if(isspace(str[i])){

        int zashel = 0;
        while(isspace(str[i]) && i < size){
            ++i;
            zashel = 1;
        }

        strs[num_of_str][cur_size] = '\"';
        cur_size++;
        strs[num_of_str] = realloc(strs[num_of_str], cur_size + 1);
        strs[num_of_str][cur_size] = '\0';

        //Check str Ending
        if(i == size) break;

        strs = realloc(strs, num_of_str + 1);
        ++num_of_str;
        cur_size = 1;
        if(zashel) --i;
        //printf("%s\n", strs[num_of_str - 1]);
        continue;
	}

	strs[num_of_str] = realloc(strs[num_of_str], cur_size+1);
    strs[num_of_str][0] = '\"';
	strs[num_of_str][cur_size] =  str[i];
    ++cur_size;
	}
    if(num_of_str != 0) ++num_of_str;
	//if(str[size-1] == '\n') --num_of_str;

	//out_str_array(strs, num_of_str);

	///Sort list of string's
	qsort(strs, num_of_str, sizeof(char *), compare_string);

    out_str_array(strs, num_of_str);


	free(str);

	for(int i = 0; i < num_of_str; ++i){
        free(strs[i]);
	}
	free(strs);


	return 0;
}
