#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

typedef struct Element Element;
struct Element
{
    char fichier[100];
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
    strcpy(e->fichier, file);
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
	    strcpy(new->fichier, file);
        new->e = l->p;
        l->p = new;
    }
    return l;
}

List* rechercherFichier(){
    DIR *dirp;
    struct dirent *entry;
    struct stat s;
    char fichier[400];
    char PATH[_SC_UCHAR_MAX];
    
    if(getcwd(PATH, _SC_UCHAR_MAX) == NULL) {
        return NULL;
    }

    
    if (!strstr(PATH,"/dir")){
        sprintf(PATH,"%s/dir/",PATH);
    }else {
        sprintf(PATH,"%s/",PATH);
    }

    
    dirp = opendir(PATH);

    List *exec = NULL;
    List *old = NULL;
    List *cible = NULL;

    while ((entry = readdir(dirp)) != NULL) {
        sprintf(fichier,"%s%s",PATH,entry->d_name);
        if (stat(fichier, &s) != -1) {
            if ((s.st_mode & S_IXUSR) && (s.st_mode & S_IFREG)){
                if (!strstr(entry->d_name,".old"))
                {
                    exec = insert(exec, fichier);
                }
                else
                {
                    old = insert(old, fichier);
                }
            }
        }
    }
    
    Element *actuel = exec->p;
    while (actuel != NULL)
    {
        if (isInfected(actuel->fichier, old) == false)
        {
            cible = insert(cible, actuel->fichier);
        }
        actuel = actuel->e;
    }

    return cible;
}

int main(){
    List* test = rechercherFichier();
    printList(test);
    return EXIT_SUCCESS;
}
