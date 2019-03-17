#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pwd.h>
#include <grp.h>

int compare_string(const void* str1, const void* str2){
    return (strcmp(*(char**)str2, *(char**)str1));
}

const int TEST = 1024;

const size_t NORMALL_LENGTH = 1e5;

int compare_strings(const void* a, const void* b)
{
    return strcmp(*(char* const*)a, *(char* const*)b);
}

enum TYPE {
    file_types,
    directory_types,
    link_types
};

char* OMAYGOD(struct stat info)
{
    char* attribute = malloc(13);
    sprintf(attribute, "%s", (S_ISDIR(info.st_mode)) ? "d" : "-");
    sprintf(attribute + 1, "%s", (info.st_mode & S_IRUSR) ? "r" : "-");
    sprintf(attribute + 2, "%s", (info.st_mode & S_IWUSR) ? "w" : "-");
    sprintf(attribute + 3, "%s", (info.st_mode & S_IXUSR) ? "x" : "-");
    sprintf(attribute + 4, "%s", (info.st_mode & S_IRGRP) ? "r" : "-");
    sprintf(attribute + 5, "%s", (info.st_mode & S_IWGRP) ? "w" : "-");
    sprintf(attribute + 6, "%s", (info.st_mode & S_IXGRP) ? "x" : "-");
    sprintf(attribute + 7, "%s", (info.st_mode & S_IROTH) ? "r" : "-");
    sprintf(attribute + 8, "%s", (info.st_mode & S_IWOTH) ? "w" : "-");
    sprintf(attribute + 9, "%s", (info.st_mode & S_IXOTH) ? "x" : "-");
    attribute[10] = '\0';
    return attribute;
}

void ls(char* dir_name){

    DIR* dir = opendir(dir_name);
    if (dir == NULL) {
        perror("Not valid dir");
        exit(-1);
    }
    printf("%s:\n", dir_name);

    //detecting file names and sort them
    size_t file_names_capacity = TEST, file_names_size = 0;
    char** file_names = malloc(sizeof(char*) * file_names_capacity);

    struct dirent* current;

    while ((current = readdir(dir)) != NULL) {
        if (!strcmp(current->d_name, ".") || !strcmp(current->d_name, "..")) {
            continue;
        }

        file_names[file_names_size] = malloc(sizeof(char*) * NORMALL_LENGTH);
        sprintf(file_names[file_names_size++], "%s", current->d_name);
    }
    closedir(dir);

    qsort(file_names, file_names_size, sizeof(char*), compare_strings);

    size_t just_directory_size = 0, just_directory_capacity = TEST; //capacity is given here
    char** just_directory = malloc(sizeof(char*) * just_directory_capacity);

    struct stat info;

    for (int i = 0; i < file_names_size; ++i) {
        char* full_filepath = malloc(NORMALL_LENGTH);

        sprintf(full_filepath, "%s/%s", dir_name, file_names[i]);

        lstat(full_filepath, &info);

        enum TYPE type;

        if (S_ISLNK(info.st_mode)) {
            type = link_types;
        }
        else if (S_ISDIR(info.st_mode)) {
            type = directory_types;
        }
        else {
            type = file_types;
        }

        char* attributes = OMAYGOD(info);
        //Аттрибуты Кол_во_ссылок Владелец Группа Размер Имя
        printf("%s %ld %s %s %li %s  \n", attributes, info.st_nlink,
        ((struct passwd *)getpwuid(info.st_uid))->pw_name,
            ((struct group*)getgrgid(info.st_gid))->gr_name, info.st_size, file_names[i]);

        if (type == directory_types) {
            if (just_directory_size + 2 > just_directory_capacity) {
                just_directory_capacity *= 2;
                just_directory = realloc(just_directory, sizeof(char*) * just_directory_capacity);
            }
            just_directory[just_directory_size] = malloc(NORMALL_LENGTH);
            sprintf(just_directory[just_directory_size++], "%s", full_filepath);
        }
    }

    printf("\n");
    for (int i = 0; i < file_names_size; ++i) {
        free(file_names[i]);
    }
    free(file_names);

    for(int i = 0; i < just_directory_size; ++i){
        ls(just_directory[i]);
    }
}

int main(int argc, char** argv){
    //argc = 2;
    //argv[1] = "/boot";

    char start_directory[NORMALL_LENGTH];

    if (argc == 2) {
        ls(argv[1]);
    }
    else if (argc == 1) {
        getcwd(start_directory, sizeof(start_directory));
        sprintf(start_directory, "%s/", start_directory);

        ls(start_directory);
    }

    return 0;
}
