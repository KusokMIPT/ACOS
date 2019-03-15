#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pwd.h>
#include <grp.h>


int compare_string (const void * str1, const void * str2) {
    return(strcmp( *(char**)str2, *(char**)str1));
}

const int TEST = 64;

const size_t NORMALL_LENGTH = 1e5;

enum TYPE {
    file_types,
    directory_types,
    link_types
};

char* OMAYGOD(struct stat info){
    char* attribute = malloc(13);
    sprintf(attribute, "%s", (S_ISDIR(info.st_mode)) ? "d" : "-");
    sprintf(attribute + 1, "%s", (info.st_mode & S_IRUSR) ? "r" : "-");
    sprintf(attribute + 2, "%s",(info.st_mode & S_IWUSR) ? "w" : "-");
    sprintf(attribute + 3, "%s",(info.st_mode & S_IXUSR) ? "x" : "-");
    sprintf(attribute + 4, "%s",(info.st_mode & S_IRGRP) ? "r" : "-");
    sprintf(attribute + 5, "%s",(info.st_mode & S_IWGRP) ? "w" : "-");
    sprintf(attribute + 6, "%s",(info.st_mode & S_IXGRP) ? "x" : "-");
    sprintf(attribute + 7, "%s",(info.st_mode & S_IROTH) ? "r" : "-");
    sprintf(attribute + 8, "%s",(info.st_mode & S_IWOTH) ? "w" : "-");
    sprintf(attribute + 9, "%s",(info.st_mode & S_IXOTH) ? "x" : "-");
    attribute[10] = '\0';
    return attribute;
}

void ls(char* dir_name){
    DIR* dir = opendir(dir_name);
    if(dir == NULL){
        perror("Not valid dir");
        exit(-1);
    }
    printf("%s:\n", dir_name);

    size_t dirs_capacity = TEST, dirs_size = 0;
    char** dirs = malloc(sizeof(char*) * dirs_capacity);

    struct dirent* current;
    struct stat info;

    while((current = readdir(dir)) != NULL){
        if(!strcmp(current->d_name, ".") || !strcmp(current->d_name, "..")){
            continue;
        }

        //detecting file name:
        size_t name_capacity = NORMALL_LENGTH, file_name_size = strlen(current->d_name);
        char* file_name = malloc(name_capacity);

        sprintf(file_name, "%s/%s", dir_name, current->d_name);
        //file_name[file_name_size++] = '\0';

        lstat(file_name, &info);

        enum TYPE type;

        if (S_ISLNK(info.st_mode)) {
              type = link_types;
            } else if (S_ISDIR(info.st_mode)) {
              type = directory_types;
            } else {
              type = file_types;
            }

        char* attributes = OMAYGOD(info);

        //Аттрибуты Кол_во_ссылок Владелец Группа Размер Имя
        printf("%s %ld %s %s %li %s  \n", attributes, info.st_nlink, getpwuid(info.st_uid)->pw_name,
            ((struct group*)getgrgid(info.st_gid))->gr_name, info.st_size, current->d_name);

        if(type == directory_types){
            if(dirs_size + 2 > dirs_capacity){
                dirs_capacity*= 2;
                dirs = realloc(dirs, sizeof(char*) * dirs_capacity);
            }
            dirs[dirs_size] = malloc(NORMALL_LENGTH);
            sprintf(dirs[dirs_size++], "%s/0", file_name);
        }
    }

    /// Пока без рекурсии
    /* Вот это: Если в качестве аргумента указано имя файла,
    то вывести информацию только об этом единственном файле (одна строка).
    */
    closedir(dir);
    for(int i = 0; i < dirs_size; ++i){
        free(dirs[i]);
    }
    free(dirs);
}

int main(int argc, char **argv){
    argc = 2;
    argv[1] = "/boot";

    char start_directory[NORMALL_LENGTH];

    if(argc == 2){
        ls(argv[1]);
    }
    else if(argc == 1){
        getcwd(start_directory, sizeof(start_directory));
        sprintf(start_directory, "%s/", start_directory);

        ls(start_directory);
    }

    return 0;
}
