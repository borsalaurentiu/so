#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define BUFF_SIZE 512
char *path;
int section;
char *sections;
int line;
char *lines;
char *recursive;
char *token;
char *perm_string;
char *sizeSmalls;
int sizeSmall=0;
char nume[BUFF_SIZE];

int listDir(char *path){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat status;
    char nume[BUFF_SIZE];
    dir = opendir(path);

    if(dir == NULL){
        perror("Invalid directory path");
        return -1;
    }

    while((entry = readdir(dir)) != NULL){
        snprintf(nume, BUFF_SIZE, "%s/%s", path, entry->d_name);
        lstat (nume, &status);

		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
            if (S_ISDIR(status.st_mode))
                printf("%s/%s\n", path,entry->d_name);
                else if (S_ISREG(status.st_mode))
                    printf("%s/%s\n",path,entry->d_name);
                    else if (S_ISLNK(status.st_mode))
                        printf("%s/%s\n",path,entry->d_name);
        }
    }

    closedir(dir);
    return 0;
}

int listRec(char *path){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat status;
    char nume[BUFF_SIZE];
    dir = opendir(path);

    if(dir == NULL){
        perror("Invalid directory path");
        return -1;
    }

    while((entry = readdir(dir)) != NULL){
        snprintf(nume, BUFF_SIZE, "%s/%s", path, entry->d_name);
        lstat (nume, &status);

        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if(S_ISDIR(status.st_mode)) {
                listRec(nume);
                printf("%s/%s\n", path, entry->d_name);
            }
                else if (S_ISREG(status.st_mode))
                    printf("%s/%s\n", path, entry->d_name);
                    else if (S_ISLNK(status.st_mode))
                        printf("%s/%s\n",path,entry->d_name);
        }
    }

    closedir(dir);
    return 0;
}

int listDirSize (char *path, int sizeSmall){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat status;
    char nume[BUFF_SIZE];
    dir = opendir(path);

    if(dir == NULL){
        perror("Invalid directory path");
        return -1;
    }

    while((entry = readdir(dir)) != NULL){
        snprintf(nume, BUFF_SIZE, "%s/%s", path, entry->d_name);
        lstat (nume, &status);

		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            if (S_ISREG(status.st_mode))
                if(sizeSmall > status.st_size)
                    printf("%s/%s\n", path, entry->d_name);
    }

    closedir(dir);
    return 0;
}

int listRecSize (char *path, int sizeSmall){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat status;
    char nume[BUFF_SIZE];
    dir = opendir(path);

    if(dir == NULL){
        perror("Invalid directory path");
        return -1;
    }

    while((entry = readdir(dir)) != NULL){
        snprintf(nume, BUFF_SIZE, "%s/%s", path, entry->d_name);
        lstat (nume, &status);

        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if(S_ISDIR(status.st_mode))
                listRecSize(nume, sizeSmall);
                else if (S_ISREG(status.st_mode))
                    if(status.st_size < sizeSmall)
                        printf("%s/%s\n", path, entry->d_name);
        }
    }
    closedir(dir);
    return 0;
}

bool print_perm_string(char *perm_string, struct stat file_stat){
	char perm[9] = "---------";
	mode_t mode = file_stat.st_mode;

	(mode & S_IRUSR) ? (perm[0] = 'r') : (perm[0] = '-');
	(mode & S_IWUSR) ? (perm[1] = 'w') : (perm[1] = '-');
	(mode & S_IXUSR) ? (perm[2] = 'x') : (perm[2] = '-');
	(mode & S_IRGRP) ? (perm[3] = 'r') : (perm[3] = '-');
	(mode & S_IWGRP) ? (perm[4] = 'w') : (perm[4] = '-');
	(mode & S_IXGRP) ? (perm[5] = 'x') : (perm[5] = '-');
	(mode & S_IROTH) ? (perm[6] = 'r') : (perm[6] = '-');
	(mode & S_IWOTH) ? (perm[7] = 'w') : (perm[7] = '-');
	(mode & S_IXOTH) ? (perm[8] = 'x') : (perm[8] = '-');
    int i = 8, adevarat = 1;
    while(i>=0)
    {
        if( perm[i] != perm_string[i] )
            adevarat = 0;
        i--;
    }
    if (adevarat != 0)
        return true;
    return false;
}

int listDirPermission (char *path, char *perm_string){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat status;
    char nume[BUFF_SIZE];
    dir = opendir(path);

    if(dir == NULL){
        perror("Invalid directory path");
        return -1;
    }

    while((entry = readdir(dir)) != NULL){
        snprintf(nume, BUFF_SIZE, "%s/%s", path, entry->d_name);
        lstat (nume, &status);

		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
            if (S_ISDIR(status.st_mode))
                {
                    if(print_perm_string(perm_string, status))
                    printf("%s/%s\n",path,entry->d_name);
                }
                else if (S_ISREG(status.st_mode))
                {

                    if(print_perm_string(perm_string, status))
                    printf("%s/%s\n",path,entry->d_name);
                }
                else if (S_ISLNK(status.st_mode))
                {

                    if(print_perm_string(perm_string, status))
                    printf("%s/%s\n",path,entry->d_name);
                }
        }
    }


    closedir(dir);
    return 0;
}

int listRecPermission (char *path, char *perm_string){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat status;
    char nume[BUFF_SIZE];
    dir = opendir(path);

    if(dir == NULL){
        perror("Invalid directory path");
        return -1;
    }
    while((entry = readdir(dir)) != NULL){
        snprintf(nume, BUFF_SIZE, "%s/%s", path, entry->d_name);
        lstat (nume, &status);

        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if(S_ISDIR(status.st_mode)){
                listRecSize(nume, sizeSmall);
                if(print_perm_string(perm_string, status))
                printf("%s/%s\n",path,entry->d_name);
            }
                else if (S_ISREG(status.st_mode)){
                if(print_perm_string(perm_string, status))
                printf("%s/%s\n",path,entry->d_name);
            }
                    else if (S_ISLNK(status.st_mode)){
                    if(print_perm_string(perm_string, status))
                printf("%s/%s\n",path,entry->d_name);
            }
        }
    }
    closedir(dir);
    return 0;
}

int parseFile(char *path) {
    int i;
    char MAGIC[4];
    short int HEADER_SIZE;
    char VERSION;
    char NO_OF_SECTIONS;
    char SECT_NAME[7];
    int SECT_TYPE;
    int SECT_SIZE;
    int SECT_OFFSET;
    int eroare=0;

    DIR * dir = NULL;
    dir = opendir(path);
	if (dir != 0) {
		printf("ERROR\nInvalid directory path\n");
		return -1;
	}

    int FILE;
    FILE = open(path, O_RDONLY);
    if(FILE == -1){
        perror("ERROR\nInvalid directory path\n");
        return -1;
    }

    lseek(FILE, 0 ,SEEK_SET);
    read(FILE, &MAGIC, 4);
    read(FILE, &HEADER_SIZE, 2);
    read(FILE, &VERSION, 1);
    read(FILE, &NO_OF_SECTIONS, 1);
    if(strcmp("zIA3",MAGIC))
        eroare=1;

    if ((VERSION < 41) || (VERSION > 114))
        eroare=2;

    if ((NO_OF_SECTIONS < 7) || (NO_OF_SECTIONS > 18))
        eroare=3;

    for (i = 1; i <= NO_OF_SECTIONS; i++) {
        read(FILE, &SECT_NAME,7);
        read(FILE, &SECT_TYPE,4);
        read(FILE, &SECT_OFFSET,4);
        read(FILE, &SECT_SIZE,4);

        switch(SECT_TYPE){
        case 24: break;
        case 81: break;
        case 92: break;
        case 45: break;
        case 34: break;
        default: eroare = 4; break;
        }

    }
    switch(eroare){
    case 0: printf("SUCCESS\n");
            printf("version=%d\n",VERSION);
            printf("nr_sections=%d\n",NO_OF_SECTIONS);
            lseek(FILE, 8, SEEK_SET);
            for(int i=1; i<= NO_OF_SECTIONS; i++){
                read(FILE, &SECT_NAME,7);
                read(FILE, &SECT_TYPE,4);
                read(FILE, &SECT_OFFSET,4);
                read(FILE, &SECT_SIZE,4);
                printf("section%d: %s %d %d\n",i, SECT_NAME, SECT_TYPE, SECT_SIZE);
            }
    break;
    case 1: printf("ERROR\nwrong magic\n");
            return -1;
    break;
    case 2: printf("ERROR\nwrong version\n");
            return -1;
    break;
    case 3: printf("ERROR\nwrong sect_nr\n");
            return -1;
    break;
    case 4: printf("ERROR\nwrong sect_types\n");
            return -1;
    break;
    }
closedir(dir);
return 0;
}


int extractFile(char *path, int sectNr, int lineNr) {
    int eroare = 0;
    DIR * dir = NULL;
    dir = opendir(path);
	if (dir != 0) {
		printf("ERROR\ninvalid file\n");
		return -1;
	}
    int FILE;
    FILE = open(path, O_RDONLY);
    if(FILE == -1){
        perror("ERROR\ninvalid file\n");
        return -1;
    }

    switch(eroare){
    case 0: printf("SUCCESS\n");

    break;
    case 1: printf("ERROR\ninvalid file\n");
            return -1;
    break;
    case 2: printf("ERROR\ninvalid section\n");
            return -1;
    break;
    case 3: printf("ERROR\ninvalid line\n");
            return -1;
    break;
    }
closedir(dir);
return 0;
}


int main(int argc, char **argv){

        if(strstr(argv[1], "variant")){
            printf("29258");
        }

        int dir_path = 0, recursiv = 0, permisiuni = 0, dimensiune = 0, i = 0, j = 0;

        for(i=0;i<argc;i++){
            if(strstr(argv[i], "path="))
                dir_path = 1;
            if(strstr(argv[i], "recursive"))
                recursiv = 1;
            else if(strstr(argv[i], "permissions="))
                permisiuni = 1;
            else if (strstr(argv[i], "size_smaller="))
                dimensiune = 1;
        }

        if(strstr(argv[1], "list"))
            if(dir_path == 1){
                if(recursiv == 0 && permisiuni == 0 && dimensiune == 0){
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            printf("SUCCESS\n");
                            listDir(path);
                        }
                    }
                }
                if(recursiv == 1 && permisiuni == 0 && dimensiune == 0){
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            printf("SUCCESS\n");
                            listRec(path);
                        }
                    }
                }
                if(recursiv == 0 && permisiuni == 1 && dimensiune == 0){
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "permissions=")){
                            token = strtok(argv[j],"=");
                            perm_string = strtok(NULL,"=");
                            }
                    }
                            printf("SUCCESS\n");
                            listDirPermission(path, perm_string);
                }
                if(recursiv == 1 && permisiuni == 1 && dimensiune == 0){
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "permissions=")){
                            token = strtok(argv[j],"=");
                            perm_string = strtok(NULL,"=");
                            }
                    }
                            printf("SUCCESS\n");
                            listRecPermission(path, perm_string);
                }
                if(recursiv == 0 && permisiuni == 0 && dimensiune == 1){
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "size_smaller=")){
                            token = strtok(argv[j],"=");
                            sizeSmalls = strtok(NULL,"=");
                            sizeSmall = atoi(sizeSmalls);
                            }
                    }
                            printf("SUCCESS\n");
                            listDirSize(path, sizeSmall);
                }
                if(recursiv == 1 && permisiuni == 0 && dimensiune == 1){
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "size_smaller=")){
                            token = strtok(argv[j],"=");
                            sizeSmalls = strtok(NULL,"=");
                            sizeSmall = atoi(sizeSmalls);
                            }
                    }
                            printf("SUCCESS\n");
                            listRecSize(path, sizeSmall);
                }
            }




        if(strstr(argv[1], "parse"))
            if(dir_path == 1){
                for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            parseFile(path);
                            }
                }
            }


        if(strstr(argv[1], "extract")){
            if(dir_path == 1){
                for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                            if(strstr(argv[j], "section=")){
                            token = strtok(argv[j],"=");
                            sections = strtok(NULL,"=");
                            section = atoi(sections);
                            }
                            if(strstr(argv[j], "line=")){
                            token = strtok(argv[j],"=");
                            lines = strtok(NULL,"=");
                            line = atoi(lines);
                            }

                            printf("SUCCESS");
                            extractFile(path, section, line);
                }
            }
        }


return 0;
}
