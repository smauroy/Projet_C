// Inclut GTK pour créer et gérer l'application graphique.
#include <gtk/gtk.h>

#include "ui/editor.h"

// Construit la fenêtre principale quand GTK active l'application.
static void
activate(GtkApplication *app, gpointer user_data)
{
    (void)user_data; /* GTK transmet ce paramètre, mais nous n'en avons pas besoin. */

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "AutoTranslate");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 700);

    // L'interface de l'éditeur est définie dans le module ui/editor.
    gtk_window_set_child(GTK_WINDOW(window), editor_new());
    gtk_window_present(GTK_WINDOW(window));
}

// Point d'entrée : crée l'application, lance sa boucle d'événements et libère
// la référence détenue par ce fichier avant de retourner son code de sortie.
int
main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new(
        "fr.autotranslate.editor", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
