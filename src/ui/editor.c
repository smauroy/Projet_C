#include "editor.h"

#include "../core/sentence_parser.h"

/* Structure regroupant les widgets nécessaires au fonctionnement de l'éditeur */
typedef struct {
    GtkTextView *preview;
    GtkLabel *status;
    GtkDropDown *source, *target;
} EditorState;

/* Récupère le texte contenu dans le buffer de la zone de saisie */
static gchar *
get_source_text(GtkTextBuffer *buffer)
{
    /* Lit tout le texte sans modifier le document que l'utilisateur écrit. */
    GtkTextIter start;
    GtkTextIter end;

    /* Identifie le début et la fin du texte contenu dans le buffer */
    gtk_text_buffer_get_bounds(buffer, &start, &end);

    /* Extrait le texte et retourne une chaîne de caractères */
    return gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
}

/* Fonction appelée lorsque le contenu de la zone de saisie est modifié */
static void
on_source_changed(GtkTextBuffer *buffer, gpointer user_data)
{
    /* Récupère les données de l'éditeur */
    EditorState *state = user_data;
    if (state == NULL || state->preview == NULL || state->status == NULL)
        return;

    /* Récupère le texte saisi dans la zone de texte */
    gchar *source_text = get_source_text(buffer);
    /* Le découpage est séparé de GTK et peut donc être testé seul. */
    GPtrArray *sentences = sentence_parser_extract_completed(source_text);
    /* Crée une chaîne de caractères destinée à contenir l'aperçu des phrases */
    GString *preview_text = g_string_new(NULL);

    /* Parcourt toutes les phrases détectées dans le texte */
    for (guint i = 0; i < sentences->len; i++) {
        const gchar *sentence = g_ptr_array_index(sentences, i);
        if (preview_text->len > 0)
            g_string_append(preview_text, "\n\n");
        /* Ce texte indique qu'une traduction reste à faire. */
        g_string_append_printf(preview_text,
            "Original:\n%s\n\nTraduction:\n[Traduction en attente]", sentence);
    }

    /* Récupère le buffer associé à la zone d'aperçu */
    GtkTextBuffer *preview_buffer =
    gtk_text_view_get_buffer(state->preview);
    /* Met à jour le contenu de l'aperçu avec les phrases détectées */
    gtk_text_buffer_set_text(preview_buffer, preview_text->str, -1);

    /* Crée un texte indiquant le nombre de phrases terminées */
    gchar *status_text = g_strdup_printf("%u phrase%s terminée%s",
        sentences->len, sentences->len > 1 ? "s" : "",
        sentences->len > 1 ? "s" : "");
    /* Met à jour le compteur affiché dans l'interface */
    gtk_label_set_text(state->status, status_text);

    /* Libère la mémoire utilisée par les différentes chaînes et structures */
    g_free(status_text);
    g_string_free(preview_text, TRUE);
    g_ptr_array_unref(sentences);
    g_free(source_text);
}

/* Crée une zone de texte et configure ses propriétés */
static GtkWidget *
create_text_area(gboolean editable, const gchar *placeholder)
{
    /* Une seule fonction configure les deux zones de texte de l'écran. */
    GtkWidget *view = gtk_text_view_new();
    /* Définit le mode de retour à la ligne du texte */
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    /* Définit si la zone de texte peut être modifiée */
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), editable);
    /* Affiche ou masque le curseur selon si la zone est modifiable ou non */
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), editable);
    /* Définit les marges de la zone de texte */
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 12);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 12);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(view), 12);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(view), 12);

    /* Si un texte initial est fourni, l'insère dans le buffer de la zone de texte */
    if (placeholder != NULL)
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(view)),
            placeholder, -1);

    return view;
}

/* Crée un conteneur permettant le défilement d'une zone de texte */
static GtkWidget *
create_scrolled_text(GtkWidget *view)
{
    /* Crée un conteneur permettant de faire défiler le contenu d'un widget */
    GtkWidget *scroll = gtk_scrolled_window_new();
    /* Définit l'apparition automatique des barres de défilement */
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    /* Intègre la zone de texte dans le conteneur de défilement */
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), view);
    /* Autorise le conteneur à s'étendre horizontalement et verticalement */
    gtk_widget_set_hexpand(scroll, TRUE);
    gtk_widget_set_vexpand(scroll, TRUE);

    return scroll;
}

/* Fonction appellée par GTK pour le changement de langue */
static void 
on_languages_changed(GObject *objet, GParamSpec *pspec, gpointer user_data) {
        
    (void)pspec;    /* Détail technique dont on a pas besoin */

    /* Récupère les menus déroulants */
    GtkDropDown *source = GTK_DROP_DOWN(objet);
    GtkDropDown *target = GTK_DROP_DOWN(user_data);

    /* Récupère l'indice de la langue sélectionnée */
    guint number = gtk_drop_down_get_selected(source);
    /* Vérifie qu'une langue valide a bien été sélectionnée */
    if (number == GTK_INVALID_LIST_POSITION)
        return;
    /* Sélectionne automatiquement la langue opposée dans l'autre menu */
    gtk_drop_down_set_selected(target, 1 - number);
}

/* Fonction appelée lors du clic sur le bouton Traduire */
static void 
clicked_and_translate(GtkButton *button, gpointer user_data)
{
    (void)button;

    /* Récupère la zone de saisie */
    GtkTextView *source_view = GTK_TEXT_VIEW(user_data);
    /* Récupère le buffer contenant le texte saisi par l'utilisateur */
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(source_view);
    /* Extrait le texte original à l'aide de la fonction commune */
    gchar *trad_text = get_source_text(buffer);

    /* Affiche le texte récupéré et libère la mémoire allouée */
    g_print("Traduire :\n%s\n", trad_text);
    g_free(trad_text);
}

/* Crée et initialise l'ensemble de l'interface graphique de l'application */
GtkWidget *
editor_new(void)
{
    /* La fenêtre principale reçoit cette boîte comme interface complète. */
    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    /* Définit les marges */
    gtk_widget_set_margin_top(root, 20);
    gtk_widget_set_margin_bottom(root, 16);
    gtk_widget_set_margin_start(root, 20);
    gtk_widget_set_margin_end(root, 20);

    /* Crée le titre principal de l'application */
    GtkWidget *heading = gtk_label_new("AutoTranslate — Français / Vietnamien");
    gtk_widget_add_css_class(heading, "title-2");
    gtk_widget_set_halign(heading, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(root), heading);

    /* Crée un widget horizontal pour accueillir les deux menus de langue */
    GtkWidget *language_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    /* Un seul tableau sera partagé par les deux menus déroulants */ 
    static const gchar * const source_languages[] = {"Français", "Vietnamien", NULL};
    /* Crée les deux menus déroulants à partir du tableau des langues */
    GtkWidget *source = gtk_drop_down_new_from_strings(source_languages);
    GtkWidget *target = gtk_drop_down_new_from_strings(source_languages);
    /* Change la langue initial du 2e menu */
    gtk_drop_down_set_selected(GTK_DROP_DOWN(target), 1);
    /* Autorise les menus à s'étendre horizontalement */
    gtk_widget_set_hexpand(source, TRUE);
    gtk_widget_set_hexpand(target, TRUE);
    /* Connecte les menus au signal de changement de sélection */
    g_signal_connect(source, "notify::selected",G_CALLBACK(on_languages_changed), target);
    g_signal_connect(target, "notify::selected",G_CALLBACK(on_languages_changed), source);
    /* Intègre les deux menus et la flèche dans le widget horizontal */
    gtk_box_append(GTK_BOX(language_row), source);
    gtk_box_append(GTK_BOX(language_row), gtk_label_new("→"));
    gtk_box_append(GTK_BOX(language_row), target);
    /* Intègre le widget horizontal dans la fenêtre principale */
    gtk_box_append(GTK_BOX(root), language_row);

    /* Crée et ajoute le titre de la zone de saisie */
    GtkWidget *source_label = gtk_label_new("Texte original");
    gtk_widget_set_halign(source_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(root), source_label);

    /* Crée la zone de saisie modifiable par l'utilisateur */
    GtkWidget *source_view = create_text_area(TRUE, NULL);
    /* Intègre la zone de saisie dans un conteneur avec défilement */
    gtk_box_append(GTK_BOX(root), create_scrolled_text(source_view));

    /* Crée le bouton permettant de déclencher la traduction */
    GtkWidget *button = gtk_button_new_with_label("Traduire");
    /* Intègre le bouton dans la fenêtre principale */
    gtk_box_append(GTK_BOX(root), button);
    /* Connecte le clic sur le bouton à la fonction de traduction et transmet le texte de la zone de saisie*/
    g_signal_connect(button,"clicked",G_CALLBACK(clicked_and_translate),source_view);

    /* Crée et ajoute le titre de la zone d'aperçu */
    GtkWidget *preview_label = gtk_label_new("Aperçu des phrases terminées");
    gtk_widget_set_halign(preview_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(root), preview_label);

    /* Crée la zone d'aperçu non modifiable par l'utilisateur */
    GtkWidget *preview_view = create_text_area(FALSE, NULL);
    /*  Intègre la zone d'aperçu dans un conteneur avec défilement */
    gtk_box_append(GTK_BOX(root), create_scrolled_text(preview_view));

    /* Crée le compteur indiquant le nombre de phrases terminées */
    GtkWidget *status_label = gtk_label_new("0 phrase terminée");
    gtk_widget_set_halign(status_label, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(root), status_label);

    /* L'état reste valide aussi longtemps que la boîte existe. */
    EditorState *state = g_new0(EditorState, 1);
    /* Associe à la structure les widgets nécessaires à la mise à jour de l'aperçu */
    state->preview = GTK_TEXT_VIEW(preview_view);
    state->status = GTK_LABEL(status_label);
    state->source = GTK_DROP_DOWN(source);
    state->target = GTK_DROP_DOWN(target);

    /* Attache la structure à la fenêtre et prévoit la libération de la mémoire allouée */
    g_object_set_data_full(G_OBJECT(root), "autotranslate-editor-state",
        state, g_free);
    /* Connecte les modifications du texte au callback chargé de mettre à jour l'aperçu */
    g_signal_connect(gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)),
        "changed", G_CALLBACK(on_source_changed), state);

    return root;
}
