#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define NB_CIBLES 10

typedef struct Element Element;
struct Element
{
    char fichier[100];
    Element *next;
    Element *prev;
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
        actuel = actuel->prev;
    }
    return false;
}

void printList(List *liste)
{
    if (liste == NULL)
    {
        return;
    }
    else
    {
        Element *actuel = liste->p;
        while (actuel != NULL)
        {
            printf("%s\n", actuel->fichier);
            actuel = actuel->prev;
        }
    }
}

List *init(char * file)
{
    List *list = malloc(sizeof(*list));
    Element *newElem = malloc(sizeof(*newElem));
    if (list == NULL || newElem == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(newElem->fichier, file);
    newElem->next = NULL;
    newElem->prev = NULL;
    list->p = newElem;
    return list;
}

List* insert(List *list, char* file)
{
    if (list == NULL){
        list = init(file);
        return list;
    }else{
        Element *newElem = malloc(sizeof(*newElem));
        if (list == NULL || newElem == NULL)
        {
            exit(EXIT_FAILURE);
        }
	    strcpy(newElem->fichier, file);
        if (!list && !list->p) {
            list->p->next = newElem;
        }
        newElem->prev = list->p;
        newElem->next = NULL;
        list->p = newElem;
    }
    return list;
}

bool    deleteItem(List *list, Element *deleteItem) {
    if (!list && !deleteItem)
        return false;

    if (deleteItem->next)
        deleteItem->next->prev = deleteItem->prev;
    if (deleteItem->prev)
        deleteItem->prev->next = deleteItem->next;

    if (list->p == deleteItem) {
        if (deleteItem->next)
            list->p = deleteItem->next;
        else if (deleteItem->prev)
            list->p = deleteItem->prev;
    }

    deleteItem->next = NULL;
    deleteItem->prev = NULL;
    free(deleteItem);
    return true;
}

List* recursiveResearch(char* PATH, int cibles){
    DIR *dirp;
    struct dirent *entry;
    struct stat s;

    dirp = opendir(PATH);
    List *fichiers = NULL;
    List *old = NULL;
    char fichier[500];

    while ((entry = readdir(dirp)) != NULL) {
        
        sprintf(fichier,"%s/%s",PATH,entry->d_name);
        

        if (stat(fichier, &s) != -1) {
            if (S_ISDIR(s.st_mode) && !strstr(entry->d_name,".") && !strstr(entry->d_name,"..") && cibles < NB_CIBLES ) {
                recursiveResearch(fichier, cibles);
            }
            if ((s.st_mode & S_IXUSR) && (s.st_mode & S_IFREG)){
                if (!strstr(entry->d_name,".old") && cibles < NB_CIBLES) //->Passe avec "MonPg1.old.qqch" ou ".olderFile" (. étant le fichier caché)
                {
                    //printf("Fichier : %s\n", fichier);
                    fichiers = insert(fichiers, fichier);
                    cibles++;
                } 
                else
                {
                    old = insert(old, fichier);
                }
            }
        }
    }

    if (fichiers)
    {
        Element *actuel = fichiers->p;
        while (actuel != NULL)
        {
            //printf("%s\n", actuel->fichier);
            if (isInfected(actuel->fichier, old)){
                deleteItem(fichiers, actuel);
            }
            actuel = actuel->prev;
        }
    }

    printList(fichiers);
    return fichiers;
}

int main(){
    char PATH[_SC_UCHAR_MAX];
    
    if(getcwd(PATH, _SC_UCHAR_MAX) == NULL) {
        return EXIT_FAILURE;
    }

    List* cibles = recursiveResearch(PATH, 0);

    //printList(cibles);
    return EXIT_SUCCESS;
}
