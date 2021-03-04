#include <gtk/gtk.h>
#define G_SOURCE_FUNC(f) ((GSourceFunc) (void (*)(void)) (f))
char table[2][100] = {
        "/home/mousstach/Bureau/univ-lr/L3/semestre6/univ-lr__Securite_TP1/imgs/image-attractive-960x540.jpeg",
        "/home/mousstach/Bureau/univ-lr/L3/semestre6/univ-lr__Securite_TP1/imgs/pexels-sam-willis-1154619.jpg"
};

void f(GtkWidget *button, gpointer data);

int main(int argc, char **argv){
    // Initialisation de gtk //
    gtk_init(&argc, &argv);
    // Creation de la fenetre window + dimensionnement + titre de la barre de titre //
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);
    gtk_window_set_title(GTK_WINDOW(window),"SlideShow");
    //  If we press on the x button located in the upper right corner of //
    // the titlebar, or we press Atl + F4 , a destroy signal is emitted. //
    // The gtk_main_quit() function is called, which terminates the application. //
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *thebox = gtk_box_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), thebox);
    GtkWidget *button = gtk_button_new();
    gtk_box_pack_start(GTK_BOX(thebox), button, TRUE, FALSE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(f), NULL);
    GtkWidget *image = gtk_image_new_from_file(table[0]);
    gtk_container_add(GTK_CONTAINER(button), image);
    g_object_set_data(G_OBJECT(button), "image", image);
    gtk_widget_show_all(window);
    ///// Parametre 2: This data (button actually), is typically an object which ‘owns’ the /////
    ///// timeout or idle callback, such as a widget or a network protocol implementation. /////
    g_timeout_add(4000, (GSourceFunc) f, (gpointer) button);// ajout de: g_timeout_add
    gtk_main();
    return EXIT_SUCCESS;
}

void f(GtkWidget *button, gpointer window){
    /* On récupère le pointeur vers l'image qui est dans le bouton */
    GtkWidget *image = g_object_get_data(G_OBJECT(button), "image");
    /* Effectue ce que tu veut sur l'image  */
    gtk_image_set_from_file(GTK_IMAGE(image), table[1]);
    //gtk_window_close (window);
    return;
}