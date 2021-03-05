#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define PATH "/Boulot/L3/S6/SECURITE/TP/VIRUS/programmes/"

bool isInfected(char* file, char* path){
    DIR *dirp2;
    struct dirent *ligne;
    char cible[30];

    sprintf(cible,"%s.old",file);

    dirp2 = opendir(path);
    while ((ligne = readdir(dirp2)) != NULL) {
        if (strcmp(ligne->d_name, cible) == 0)
        {
            return true;
        }
    }
    return false;
}

int main(){
    DIR *dirp;
    struct dirent *entry;

    struct stat s;
    char fichier[300];
    dirp = opendir(PATH);

    printf("S_IXUSR: %d\n", S_IXUSR);
    printf("S_IFREG: %d\n", S_IFREG);

    while ((entry = readdir(dirp)) != NULL) {
        if(entry->d_type == 0 && entry->d_name[6] == '\0') {
            printf("Name: %s\tType: %d\t", entry->d_name, entry->d_type);
            sprintf(fichier,"%s%s",PATH,entry->d_name);

            if (stat(fichier, &s) != -1) {
                printf("Mode: %hu\n", s.st_mode);
            }


            if (isInfected(entry->d_name, PATH) == true){
                printf("Fichier %s deja infecte !\n\n", entry->d_name);
            }
            else
            {
                printf("Fichier %s non infecte !\n\n", entry->d_name);
            }
        }
    }
}