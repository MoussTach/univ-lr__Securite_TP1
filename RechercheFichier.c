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
        if(entry->d_type == 0 && !strstr(entry->d_name,".old")) { // 8 sur Linux
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


/*
typedef struct Element Element;
struct Element
{
    char* fichier;
    Element *e;
};

typedef struct List List;
struct List
{
    Element *p;
};

bool isInfected(char* file, List *liste)
{
    if (liste == NULL)
    {
        return false;
    }

    char cible[200];
    sprintf(cible,"%s.old",file);

    Element *actuel = liste->p;

    while (actuel != NULL)
    {
        if (strcmp(cible, actuel->fichier) == 0)
        {
            return true;
        }
        actuel = actuel->e;
    }

    return false;
}

void printList(List *liste)
{
    if (liste == NULL)
    {
        printf("Liste Vide \n");
    }
    else
    {
        Element *actuel = liste->p;

        while (actuel != NULL)
        {
            printf("%s\n", actuel->fichier);
            actuel = actuel->e;
        }
    }
}

List *init(char * file)
{
    List *l = malloc(sizeof(*l));
    Element *e = malloc(sizeof(*e));

    if (l == NULL || e == NULL)
    {
        exit(EXIT_FAILURE);
    }

    e->fichier = file;
    e->e = NULL;
    l->p = e;

    return l;
}

List* insert(List *l, char* file)
{
    if (l == NULL){
        l = init(file);
        return l;
    }else{
        Element *new = malloc(sizeof(*new));
        if (l == NULL || new == NULL)
        {
            exit(EXIT_FAILURE);
        }
        new->fichier = file;

        new->e = l->p;
        l->p = new;
    }
    return l;
}

int main(){
    DIR *dirp;
    struct dirent *entry;

    struct stat s;
    char fichier[300];
    dirp = opendir(PATH);

    List *exec = NULL;
    List *old = NULL;


    while ((entry = readdir(dirp)) != NULL) {
        sprintf(fichier,"%s%s",PATH,entry->d_name);

        if (stat(fichier, &s) != -1) {
            if ((s.st_mode & S_IXUSR) && (s.st_mode & S_IFREG)){
                if (!strstr(entry->d_name,".old"))
                {
                    insert(exec, fichier);
                }
                else
                {
                    insert(old, fichier);
                }
            }
        }
    }
    printList(exec);
    printf("fin\n");
    return EXIT_SUCCESS;
}
*/
