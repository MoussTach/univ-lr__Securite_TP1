#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Element Element;
struct Element
{
    int nb;
    Element *e;
};

typedef struct List List;
struct List
{
    Element *p;
};

// ----------------------------------- LISTE CHAINEE -----------------------------------

void printList(List *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->p;

    while (actuel != NULL)
    {
        printf("%d -> ", actuel->nb);
        actuel = actuel->e;
    }
    printf("NULL\n");
}

List *init()
{
    List *l = malloc(sizeof(*l));
    Element *e = malloc(sizeof(*e));

    if (l == NULL || e == NULL)
    {
        exit(EXIT_FAILURE);
    }

    e->nb = 2;
    e->e = NULL;
    l->p = e;

    return l;
}

void insert(List *l, int nb)
{
    Element *new = malloc(sizeof(*new));
    if (l == NULL || new == NULL)
    {
        exit(EXIT_FAILURE);
    }
    new->nb = nb;

    new->e = l->p;
    l->p = new;
}

// ----------------------------------- NOMBRE PREMIER -----------------------------------

char isPrime(int n){
    int i;
    for(i = 2 ; i <= sqrt(n) ; i++)
    {
        if(n%i == 0) return '0';
    }
    return '1';
}

// ----------------------------------- MAIN -----------------------------------

int main(int argc, char **argv)
{
    int i, nbr, tmp;
    List *result = init();

    printf("--------------------- MonPG1 ---------------------\n\n");
    printf("Ce programme permet de recuperer une liste de nombre premier.\n\n"
           "Parametre du programme : \n- Un nombre entier N."
           "\n\n");
    printf("--------------------------------------------------\n\n");

    printf("Entrez un nombre entier N superieur a 2 :");
    scanf("%d", &nbr);

    tmp = (int)nbr;

    if(tmp == nbr && nbr > 2)
    {
        for(i = 3 ; i < nbr ; i++)
        {
            if(isPrime(i) == '1')
            {
                insert(result,i);
            }
        }
        printf("Liste des nombres premiers entre 2 et %d :\n\n", nbr);
        printList(result);
    }
    else
    {
        printf("Erreur le nombre choisit ne repond pas aux criteres !");
    }

    printf("\n--------------------------------------------------\n\n");
    return 0;
}
