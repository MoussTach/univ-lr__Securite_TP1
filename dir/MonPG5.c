#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define NOIR 0
#define ROUGE 1

typedef struct arbre_ABC{
    struct arbre_ABC *left, *right, *parent;
    int color;
    int data;
}Node;

typedef struct arbre_AVL{
    int data;
    int h;
    struct arbre_AVL *left,*right;
}AVL;

void abc_create(Node **node, int number);
Node* getParent(Node* n);
Node* getGrandParent(Node* n);
Node* getFrere(Node* n);
Node* getOncle(Node* n);
Node* getRoot(Node *n);
void left_rotate(Node *x);
void right_rotate(Node *x);
Node* insert(Node* root, Node* n);
void insertRecurse(Node* root, Node* n);
void repair_tree(Node* n);
void Case4(Node* n);
void Case4Step2(Node* n);
Node* findNode(Node* root, int x);
Node* minimum(Node *x);
Node* successeur(Node *x);
void supprimer(Node *T, Node *z);
void supprimer_correction(Node* T,Node* x);
void abc_delete(Node *n);
void abc_print(Node **abc, unsigned int palt);
void abc_print_color(Node **abc,unsigned int palt);
void inorder(Node *tree);
size_t abc_validate(Node *tree);
void avl_create(AVL **avl,int number);
int max(int a, int b);
int avl_height(AVL **avl);
AVL* avl_insert2(AVL **avl,int number);
void avl_insert(AVL **avl,int number);
void avl_equilibre(AVL **avl);
AVL* rotSimpleR(AVL **avl);
AVL* rotSimpleL(AVL **avl);
void rotDoubleRightLeft(AVL **avl);
void rotDoubleLeftRight(AVL **avl);
void avl_delete(AVL **avl);
AVL* avl_remove(AVL* t, int x);
AVL* findMin(AVL* t);
void avl_print(AVL **avl, unsigned int palt);

//----------------------------------------

void abc_create(Node **node, int number){
    Node *new_node = NULL;

    new_node = (Node*)malloc(sizeof(Node));
    if(new_node){
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->parent = NULL;
        new_node->color = NOIR;
        new_node->data = number;
        *node = new_node;
    }
}

Node* getParent(Node* n) {return n == NULL ? NULL : n->parent;}

//----------------------------------------

Node* getGrandParent(Node* n) {return getParent(getParent(n));}

//----------------------------------------

Node* getFrere(Node* n) {
    Node* p = getParent(n);
    if (p == NULL) {
        return NULL;
    }
    if (n == p->left) {
        return p->right;
    } else {
        return p->left;
    }
}

//----------------------------------------
Node* getOncle(Node* n) {
    Node* p = getParent(n);
    return getFrere(p);
}

//----------------------------------------

Node* getRoot(Node *n){
    if(n->parent == NULL){
        return n;
    } else {
        return getRoot(n->parent);
    }
}

//----------------------------------------

void left_rotate(Node *x) {
    Node *y = x->right;
    Node *root = getRoot(x);
    x->right = y->left;
    if (y->left != NULL){
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL){
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

//----------------------------------------

void right_rotate(Node *x) {
    Node *y = x->left;
    Node *root = getRoot(x);
    x->left = y->right;
    if (y->right != NULL){
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL){
        root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

//----------------------------------------

Node* insert(Node* root, Node* n) {
    insertRecurse(root, n);
    repair_tree(n);
    while (getParent(n)) {
        n = getParent(n);
    }
    return n;
}

//----------------------------------------

void insertRecurse(Node* root, Node* n) {
    if (root != NULL)
    {
        if (n->data < root->data) {
            if (root->left) {
                insertRecurse(root->left, n);
                return;
            } else {
                root->left = n;
            }
        } else {
            if (root->right) {
                insertRecurse(root->right, n);
                return;
            } else {
                root->right = n;
            }
        }
    }

    n->parent = root;
    n->left = NULL;
    n->right = NULL;
    n->color = ROUGE;
}

//----------------------------------------

void repair_tree(Node* n) {
    if (getParent(n) == NULL) {
        n->color = NOIR;
    } else if (getParent(n)->color == NOIR) {
        return;
    } else if (getOncle(n) && getOncle(n)->color == ROUGE) {
        getParent(n)->color = NOIR;
        getOncle(n)->color = NOIR;
        getGrandParent(n)->color = ROUGE;
        repair_tree(getGrandParent(n));
    } else {
        Case4(n);
    }
}

//----------------------------------------

void Case4(Node* n) {
    Node* p = getParent(n);
    Node* g = getGrandParent(n);

    if (n == p->right && p == g->left) {
        left_rotate(p);
        n = n->left;
    } else if (n == p->left && p == g->right) {
        right_rotate(p);
        n = n->right;
    }

    Case4Step2(n);
}

//----------------------------------------

void Case4Step2(Node* n) {
    Node* p = getParent(n);
    Node* g = getGrandParent(n);

    if (n == p->left) {
        right_rotate(g);
    } else {
        left_rotate(g);
    }
    p->color = NOIR;
    g->color = ROUGE;
}

//----------------------------------------

Node* findNode(Node* root, int x){
    if (root->data == x){
        return root;
    } else {
        if (root->data > x) {
            return findNode(root->left,x);
        } else {
            return findNode(root->right,x);
        }
    }
}

//----------------------------------------

Node* minimum(Node *x) {
    while (x->left) {
        x = x->left;
    }
    return x;
}

//----------------------------------------

Node* successeur(Node *x) {
    if (x->right){
        return minimum(x->right);
    }
    Node *y = x->parent;
    while(y && x==y->right) {
        x = y;
        y = y->parent;
    }
    return x;
}

//----------------------------------------

void supprimer(Node *T, Node *z) {
    Node *x = NULL;
    Node *y = NULL;
    if (z->left==NULL || z->right==NULL) {
        y = z;
    } else {
        y = successeur(z);
        printf("Valeur y : %d\n", y->data);

    }
    if (y->left == NULL && y->right == NULL) {
        x = y;
    } else {
        if(y->left) {
            x = y->left;
        } else {
            x = y->right;
        }
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        T = x;
    }
    else if (y==y->parent->left) {
        y->parent->left = x;
    }
    else {
        y->parent->right = x;
    }
    if (y!=z) {
        z->data = y->data;
    }
    if (y->color == NOIR) {
        supprimer_correction(T,x);
    }
    if(y->parent->left == y) {
        y->parent->left=NULL;
    }
    if(y->parent->right == y) {
        y->parent->right=NULL;
    }
    free(y);
}

//----------------------------------------

void supprimer_correction(Node* T,Node* x) {
    Node *s = NULL;
    while (x != T && x->color == NOIR){
        if (x->parent && x == x->parent->left) {
            s = x->parent->right;
            if (s->color == ROUGE) {
                s->color = NOIR;
                x->parent->color = ROUGE;
                left_rotate(x->parent);
                s = x->parent->right;
            }
            if ((s->left==NULL && s->right==NULL) || (s->left->color == NOIR && s->right->color == NOIR)) {
                s->color = ROUGE;
                x = x->parent;
            }
            else if (s->right==NULL || s->right->color == 0) {
                s->left->color = NOIR;
                s->color = ROUGE;
                right_rotate(s);
                s = x->parent->right;
            }
            s->color = x->parent->color;
            x->parent->color = NOIR;
            if (s->right){
                s->right->color = NOIR;
            }
            left_rotate(x->parent);
            x = T;
        }
        else {
            s = x->parent->left;
            if (s->color == ROUGE) {
                s->color = NOIR;
                x->parent->color = ROUGE;
                right_rotate(x->parent);
                s = x->parent->left;
            }
            if ((s->left==NULL && s->right==NULL) || (s->left->color == NOIR && s->right == NOIR)) {
                s->color = ROUGE;
                x = x->parent;
            }
            else if (s->left==NULL || s->left->color == NOIR) {
                s->right->color = NOIR;
                s->color = ROUGE;
                left_rotate(s);
                s = x->parent->left;
            }
            s->color = x->parent->color;
            x->parent->color = NOIR;
            if (s->left){
                s->left->color = NOIR;
            }
            right_rotate(x->parent);
            x = T;
        }
    }
    x->color = NOIR;
}

//----------------------------------------

void abc_delete(Node *r)
{

    if (r)
    {
        abc_delete(r->left);
        abc_delete(r->right);
        free(r);
    }
}

//----------------------------------------

void abc_print(Node **abc,unsigned int palt){
    unsigned int I;

    if(*abc == NULL)
    {
        for(I=0; I < palt;I++)
        {
            printf("\t");
        }
        printf("*\n");
        return;
    }

    abc_print(&(*abc)->right,palt+1);

    for(I=0 ; I < palt;I++) printf("\t");

    printf("[%d]\n",(*abc)->data);
    abc_print(&(*abc)->left, palt+1);
}

//----------------------------------------

void abc_print_color(Node **abc,unsigned int palt){
    unsigned int I;

    if(*abc == NULL)
    {
        for(I=0; I < palt;I++)
        {
            printf("\t");
        }
        printf("*\n");
        return;
    }

    abc_print_color(&(*abc)->right,palt+1);

    for(I=0 ; I < palt;I++) printf("\t");

    printf("[%d]\n",(*abc)->color);
    abc_print_color(&(*abc)->left, palt+1);
}

//----------------------------------------

void inorder(Node *tree)
{
    if (tree == NULL)
        return;
    inorder(tree->left);
    printf("%d ", tree->data);
    inorder(tree->right);
}

//----------------------------------------

size_t abc_validate(Node *tree) {
    if (tree) {
        if (tree->left) {assert(tree->left->data <= tree->data);}
        if (tree->right) {assert((tree->data >= tree->right->data) );}
        assert(tree->color == NOIR || tree->color == ROUGE);
        if (tree->parent == NULL) {assert(tree->color == NOIR);}
        if (tree->color == ROUGE) {assert(tree->left->color == NOIR && tree->right->color == NOIR);}
        size_t black_height_left = abc_validate(tree->left);
        size_t black_height_right = abc_validate(tree->right);
        assert(black_height_left == black_height_right);
        return tree->color == NOIR ? black_height_left + 1 : black_height_left;
    }
    else {return 1;}
}

//----------------------------------------

void avl_create(AVL **avl,int number){
    AVL *new_avl = NULL;

    new_avl = (AVL*)malloc(sizeof(AVL));
    if(new_avl){
        new_avl->left = NULL;
        new_avl->right = NULL;
        new_avl->data = number;
        new_avl->h = 1;
        *avl = new_avl;
    }
}

//----------------------------------------
int max(int a, int b){
    if(a>b) return a;
    else return b;
}

//----------------------------------------

int avl_height(AVL **avl){
    if(!*avl) return 0;
    return 1+max(avl_height(&(*avl)->left),avl_height(&(*avl)->right));
}


//----------------------------------------

AVL* avl_insert2(AVL **avl,int number){
    AVL *aux =NULL;

    if(*avl){
        if((*avl)->data == number)return *avl;
        else if(number < (*avl)->data){
            (*avl)->left = avl_insert2(&((*avl)->left),number);
            avl_equilibre(&(*avl));
            return *avl;
        }
        else if(number > (*avl)->data){
            (*avl)->right = avl_insert2(&((*avl)->right),number);
            avl_equilibre(&(*avl));
            return *avl;
        }
    }
    aux = (AVL*) malloc(sizeof(AVL));
    aux->left = NULL;
    aux->right = NULL;
    aux->data = number;
    aux->h = 1;
    return aux;
}

//----------------------------------------

void avl_insert(AVL **avl,int number){
    if(*avl){
        if((*avl)->data == number)return ;
        else if(number < (*avl)->data){
            (*avl)->left = avl_insert2(&((*avl)->left),number);
            avl_equilibre(&(*avl));
        }
        else if(number > (*avl)->data){
            (*avl)->right = avl_insert2(&((*avl)->right),number);
            avl_equilibre(&(*avl));
        }
    }
}



//----------------------------------------


void avl_equilibre(AVL **avl){
    int hL;
    int hR;
    if(*avl){
        hL = avl_height(&(*avl)->left);
        hR = avl_height(&(*avl)->right);
        if(hL - hR == 2){
            hL = avl_height(&(*avl)->left->left);
            hR = avl_height(&(*avl)->left->right);
            if(hL>hR) rotSimpleR(&(*avl));
            else  rotDoubleLeftRight(&(*avl));
        }
        else if(hL - hR == -2){
            hL = avl_height(&(*avl)->right->left);
            hR = avl_height(&(*avl)->right->right);
            if(hR>hL) rotSimpleL(&(*avl));
            else rotDoubleRightLeft(&(*avl));
        }
        else {
            hL = avl_height(&(*avl)->left);
            hR = avl_height(&(*avl)->right);
            (*avl)->h = hL > hR ? hL+1 : hR+1 ;
        }
    }
}


//----------------------------------------

AVL* rotSimpleR(AVL **avl){
    AVL *aux = (*avl)->left;
    int hL;
    int hR;

    (*avl)->left = aux->right;
    aux->right = *avl;
    *avl = aux;

    hL = avl_height(&(*avl)->right->left);
    hR = avl_height(&(*avl)->right->right);
    (*avl)->right->h = hL > hR ? hL+1 : hR+1 ;

    hL = avl_height(&(*avl)->left);
    hR = avl_height(&(*avl)->right);
    (*avl)->h = hL > hR ? hL+1 : hR+1 ;
    return *avl;
}

//----------------------------------------

AVL* rotSimpleL(AVL **avl){
    AVL *aux = (*avl)->right;
    int hL;
    int hR;

    (*avl)->right = aux->left;
    aux->left = *avl;
    *avl = aux;

    hL = avl_height(&(*avl)->left->left);
    hR = avl_height(&(*avl)->left->right);
    (*avl)->left->h = hL > hR ? hL+1 : hR+1 ;

    hL = avl_height(&(*avl)->left);
    hR = avl_height(&(*avl)->right);
    (*avl)->h = hL > hR ? hL+1 : hR+1 ;
    return *avl;
}

//----------------------------------------

void rotDoubleRightLeft(AVL **avl){
    rotSimpleR(&(*avl)->right);
    rotSimpleL(&(*avl));
}

//----------------------------------------

void rotDoubleLeftRight(AVL **avl){
    rotSimpleL(&(*avl)->left);
    rotSimpleR(&(*avl));
}

//----------------------------------------

void avl_delete(AVL **avl)
{
    if (*avl)
    {
        avl_delete(&((*avl)->right));
        avl_delete(&((*avl)->left));
        free(*avl);
    }
}

//----------------------------------------

AVL* avl_remove(AVL* t, int x) {
    AVL* temp;

    if (t == NULL) return NULL;

    else if (x < t->data) t->left = avl_remove(t->left, x);
    else if (x >t->data) t->right = avl_remove(t->right, x);


    else if (t->left && t->right) {
        temp = findMin(t->right);
        t->data = temp->data;
        t->right = avl_remove(t->right, t->data);
    }

    else {
        temp = t;
        if (t->left == NULL) t = t->right;
        else if (t->right == NULL) t = t->left;
        free(temp);
    }
    if (t == NULL) return t;

    avl_equilibre(&t);
    return t;
}

//----------------------------------------

AVL* findMin(AVL* t)
{
    if (t == NULL) return NULL;
    else if (t->left == NULL) return t;
    else return findMin(t->left);
}


//----------------------------------------

void avl_print(AVL **avl,unsigned int palt){
    unsigned int I;

    if(*avl == NULL)
    {
        for(I=0; I < palt;I++)
        {
            printf("\t");
        }
        printf("*\n");
        return;
    }

    avl_print(&(*avl)->right,palt+1);

    for(I=0 ; I < palt;I++) printf("\t");

    printf("[%d]\n",(*avl)->data);
    avl_print(&(*avl)->left, palt+1);
}

void testArbresABC(void){
    srand(rand());
    Node *root = NULL;
    Node *n = NULL;
    int ch, number;
    int tableau_abc[] = {10, 100, 1000, 1500, 2000, 2500, 3000, 4000, 5000, 6000, 10000, 20000, 40000, 60000, 80000, 100000, 150000};

    while (1) {
        printf("1. Random Tree\n2. Delete node\n3. Traverse\n");
        printf("4. Affichage data\n5. Affichage color\n6. Delete Tree\n7. Compare Time (ABC vs. AVL)\n8. exit\n");
        printf("Enter your choice:");
        scanf("%d", &ch);
        switch (ch) {
            case 1:
                if (root) {
                    abc_delete(root);
                }
                abc_create(&root, rand()%100 + 1);
                for (int i = 0; i < 20; ++i)
                {
                    number = rand()%100 + 1;
                    abc_create(&n, number);
                    root = insert(root, n);
                    n = NULL;
                }
                break;
            case 2:
                if (root) {
                    printf("Enter the element to delete:");
                    scanf("%d", &number);
                    supprimer(root,findNode(root,number));
                } else {
                    printf("No tree");
                }
                break;
            case 3:
                if (root) {
                    inorder(root);
                } else {
                    printf("No tree");
                }
                printf("\n");
                break;
            case 4:
                if (root) {
                    abc_print(&root,1);
                } else {
                    printf("No tree");
                }
                printf("\n");
                break;
            case 5:
                if (root) {
                    abc_print_color(&root,1);
                } else {
                    printf("No tree");
                }
                printf("\n");
                break;
            case 6:
                if (root){
                    abc_delete(root);
                }
                printf("\n");
                break;
            case 7:
                printf("*** Temps d'execution arbre bicolore ***\n");
                for (int j = 0 ; j < 17; j++) {
                    if (root) {
                        abc_delete(root);
                        abc_create(&root, rand()%100 + 1);
                    }
                    abc_create(&root, rand()%100 + 1);
                    clock_t t0 = clock();
                    for (int i = 0; i < tableau_abc[j]; ++i)
                    {
                        number = rand()%tableau_abc[j] + 1;
                        abc_create(&n, number);
                        root = insert(root, n);
                        n = NULL;
                    }
                    clock_t t1 = clock();
                    unsigned long millis = (t1 -  t0) * 1000 / CLOCKS_PER_SEC;
                    printf("Temps d'execution pour %d noeud(s) : %ld ms\n", tableau_abc[j], millis);
                }
                abc_delete(root);
                printf("\n*** Temps d'execution arbre AVL ***\n");
                AVL *avl = NULL;
                for (int j = 0 ; j < 17; j++) {

                    if (avl) {
                        avl_delete(&avl);
                        avl_create(&avl, rand()%100 + 1);
                    }
                    avl_create(&avl, rand()%100 + 1);
                    clock_t t0 = clock();
                    for (int i = 0; i < tableau_abc[j]; ++i)
                    {
                        number = rand()%tableau_abc[j] + 1;
                        avl_insert(&avl, number);
                    }
                    clock_t t1 = clock();
                    unsigned long millis = (t1 -  t0) * 1000 / CLOCKS_PER_SEC;
                    printf("Temps d'execution pour %d noeud(s) : %ld ms\n", tableau_abc[j], millis);
                }
                avl_delete(&avl);
                break;
            case 8:
                exit(0);
            default:
                printf("Not available\n");
                break;
        }
        printf("\n");
    }

}

int main(){
    printf("--------------------- MonPG5 ---------------------\n\n");
   	printf("Ce programme permet de manipuler des arbres bicolores.\n\n"
           "Aucune paramètre à spécifié."
           "\n\n");
    	printf("--------------------------------------------------\n\n");
    testArbresABC();
    return EXIT_SUCCESS;
}