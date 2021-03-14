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
    Element *next;
    Element *prev;
};
typedef struct List List;
struct List
{
    Element *p;
};

//TODO rendre unifome le check ou en faire deux distinct entre la vérification depuis la liste "old" et "exec"
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
        printf("Liste Vide \n");
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

List* rechercherFichier(){
    DIR *dirp;
    struct dirent *entry;
    struct stat s;
    char fichier[400];
    char PATH[_SC_UCHAR_MAX];
    
    if(getcwd(PATH, _SC_UCHAR_MAX) == NULL) {
        return NULL;
    }

    //TODO spécifique à "/dir/"?
    if (!strstr(PATH,"/dir")){
        sprintf(PATH,"%s/dir/", PATH);
    }else {
        sprintf(PATH,"%s/", PATH);
    }


    //TODO -----------------------------
    //possible de découper ici la fonction pour faire la récursion
    //comme ca, on founi PATH en argument et cela va forcément chercher sur le dossier actuel
    dirp = opendir(PATH);

    List *exec = NULL;
    List *old = NULL;

    while ((entry = readdir(dirp)) != NULL) {
        sprintf(fichier,"%s%s",PATH,entry->d_name);
        if (stat(fichier, &s) != -1) {
            /*
            if (S_ISDIR(s.st_mode)) { //-> est un dossier
                if (nbFichierAffecté pas atteint) -> evite la recursion sous d'autres fichier dès le moment où on a tout ce
                                                    qu'on veut, on va simplement résoudre la recursion en regardant si il y a des .old
                    recurcive(fichier, &nbFichierAffecté)
                    //-> /!\ possible d'avoir des soucis avec le check du nombre d'executable infecté, si on devait trouver un .old par la suite si nous
                    // n'avions pas bifurqué vers un nouveau dossier
                    //TODO --> possible dans ce cas de stocker les dossier trouvé et de les traiter par la suite si notre nombre d'executable est pas atteint
            }

            */
            if ((s.st_mode & S_IXUSR) && (s.st_mode & S_IFREG)){
                if (!strstr(entry->d_name,".old")) //->Passe avec "MonPg1.old.qqch" ou ".olderFile" (. étant le fichier caché)
                {
                    //TODO possible de check si le fichier est affecté ici directement
                    //Is affected est le test d'existance entre deux listes

                    //isInfected(fichier, old) == true
                        // -> on le retire de "old" -> nous ne sommes plus sensé le retrouver
                        // -> et on le rajoute PAS dans "exec"
                    exec = insert(exec, fichier);
                }
                else
                {
                    //TODO possible de check si le fichier est affecté ici directement
                    //Is affected est le test d'existance entre deux listes
                    //si le fichier executable est retrouvé alors qu'on trouve son .old -> on le retire de "exec"
                    //sinon on le rajoute dans la liste "old"

                    //isInfected(fichier, exec) == true
                        // -> on le retire de "exec" -> nous ne sommes plus sensé le retrouver
                        // -> et on le rajoute PAS dans "old"
                    old = insert(old, fichier);
                }
            }
        }
    }
    //-------------------------------
    
    Element *actuel = exec->p;
    Element *del = NULL;
    while (actuel) {
        if (actuel && isInfected(actuel->fichier, old) == true) {
            del = actuel; //-> attention a pas se faire avoir par le décalage quand on delete sur une liste exploré
            actuel = actuel->prev;
            deleteItem(exec, del);
        } else {
            actuel = actuel->prev;
        }
    }

    return exec;
}

int main(){
    List* test = rechercherFichier();
    printList(test);
    return EXIT_SUCCESS;
}
