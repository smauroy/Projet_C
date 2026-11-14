// Intégration de la bibliothèque GTK
// Donnant accès aux fonctions, types et outils nécessaires à la création de ton interface graphique
#include <gtk/gtk.h>

// (static) Définit une fonction qui ne pourra être appelée que depuis le fichier dans lequel elle est déclarée
// (void) La fonction ne retourne pas de valeur
// (activate) Fonction appelée lors de l'activation de l'application, contenant les instructions permettant de construire et d'afficher l'interface graphique
static void activate(GtkApplication *app, gpointer data)
{
    // (data) Paramètre permettant de recevoir des données supplémentaires
    // Présent pour respecter la signature attendue par GTK, mais non utilisé dans notre programme
    (void)data;

    // Création de la fenêtre principale de l'application
    GtkWidget *window = gtk_application_window_new(app);
    // Définit le nom et les dimensions de la fenêtre
    gtk_window_set_title(GTK_WINDOW(window), "AutoTranslate");
    gtk_window_set_default_size(GTK_WINDOW(window), 850, 550);

    // Création d'une box verticale pouvant accueillir les widgets de l'interface
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    // Definit les marges de la box
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_widget_set_margin_bottom(box, 20);

    // Création d'un widget pour afficher un titre
    GtkWidget *title = gtk_label_new("AutoTranslate — Français / Vietnamien");
    // Création d'un widget permettant le défilement de son contenu
    GtkWidget *scroll = gtk_scrolled_window_new();
    // Création d'un widget intégrant une zone de texte modifiable
    GtkWidget *editor = gtk_text_view_new();
    // Création d'un widget pour l'affichage du résultat de la traduction
    GtkWidget *result = gtk_label_new("La traduction apparaîtra ici.");

    // Configuration du retour à la ligne de l'éditeur de texte
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(editor), GTK_WRAP_WORD_CHAR);
    // Intègre le widget de l'éditeur de texte dans le widget de scroll 
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), editor);
    // Permet d'étendre verticalement le widget de scroll
    gtk_widget_set_vexpand(scroll, TRUE);

    // Intègre les widgets de titre, scroll et résultat dans la box
    gtk_box_append(GTK_BOX(box), title);
    gtk_box_append(GTK_BOX(box), scroll);
    gtk_box_append(GTK_BOX(box), result);

    // Intègre le widget de la box dans la fenêtre principale
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

// Point d'entrée du programme 
// (argc) Nombres d'arguments transmis au programme
// (argv) Arguments transmis sous le type chaîne de caractères
int main(int argc, char **argv)
{
    // Création d'une application GTK sous l'identifiant "fr.autotranslate.editor" 
    // Options de configuration : par défaut
    GtkApplication *app = gtk_application_new(
        "fr.autotranslate.editor",
        G_APPLICATION_DEFAULT_FLAGS
    );

    // Connecte le signal d'activation de l'application créée à l'appel de la fonction activate
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Lance l'application et gère son fonctionnement en traitant les événements qu'elle reçoit
    // Le code de retour est récupéré dans la variable status
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    // Décrémente le compteur de référence de app (compteur = 0 → supprimé pour libérer sa mémoire)
    g_object_unref(app);

    // Retourne le code de fin d'exécution de l'application au système d'exploitation
    return status;
}
