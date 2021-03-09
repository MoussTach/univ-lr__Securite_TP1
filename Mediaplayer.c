#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gtk/gtk.h>

static char *MEDIAPLAYER = "./MediaPlayer.exe";
static char **imgs = NULL;
static int imgsSize = 0;
static int pos = -1;


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

void signalHandler(int _) {
    fprintf(stdout, "\e[91m%s\e[39m\n", "catch signal");
}

void catchSignals() {
    signal(SIGABRT, signalHandler);
    signal(SIGIO, signalHandler);
    signal(SIGPIPE, signalHandler);
    signal(SIGSEGV, signalHandler);
}

void    duplicate(char *src, char *dest) {
    char    buffer[4096];
    int     fd_src;
    int     fd_dest;
    int     bytes;

    fd_src = open(src, O_RDONLY);
    fd_dest = open(strcat(dest, ".old"), O_WRONLY | O_CREAT, 0755);

    while ((bytes = read(fd_src, buffer, 4096)) > 0) {
        if (bytes == -1) {
            fprintf(stdout, "\e[91m%s\e[39m\n", "Error reading file.");
            break;
        }
        if (write(fd_dest, buffer, bytes) == -1) {
            fprintf(stdout, "\e[91m%s\e[39m\n", "Error writing to file.");
            break;
        }
    }

    close(fd_src);
    close(fd_dest);
}

int launchDuplicate() {
    catchSignals();
    fprintf(stdout, "\e[91m%s\e[39m\n", "DUPLICATE");

    char destFile[100] = "Exe1";
    duplicate("MediaPlayer.exe", destFile);

    kill(getpid(), SIGSEGV); //TODO testing catch signal
    return 0;
}

int main(int argc, char **argv) {
    pid_t   pid;

    printf("launch\n");

    if ((pid = fork()) == -1) {
        printf("fork failed");
    } else if (pid == 0) { //Nouveau prog
        fprintf(stdout, "\e[91mpid = %d, ppid = %d\e[39m\n", getpid(), getppid());
        launchDuplicate();

    } else { //Main prog
        fprintf(stdout, "\e[91mpid = %d, ppid = %d\e[39m\n", getpid(), getppid());

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