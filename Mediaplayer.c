#include <gtk/gtk.h>


static char **imgs = NULL;
static int imgsSize = 0;
static int pos = 0;

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

int main(int argc, char **argv) {
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;

    GtkWidget *mediaPlayerWindow = NULL;

    imgs = argv + 1;
    imgsSize = argc - 1;
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