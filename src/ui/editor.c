#include "editor.h"

#include "../core/sentence_parser.h"

typedef struct {
    GtkTextView *preview;
    GtkLabel *status;
} EditorState;

static void
on_source_changed(GtkTextBuffer *buffer, gpointer user_data)
{
    EditorState *state = user_data;
    if (state == NULL || state->preview == NULL || state->status == NULL)
        return;

    /* Lit tout le texte sans modifier le document que l'utilisateur écrit. */
    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *source_text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
    /* Le découpage est séparé de GTK et peut donc être testé seul. */
    GPtrArray *sentences = sentence_parser_extract_completed(source_text);
    GString *preview_text = g_string_new(NULL);

    for (guint i = 0; i < sentences->len; i++) {
        const gchar *sentence = g_ptr_array_index(sentences, i);
        if (preview_text->len > 0)
            g_string_append(preview_text, "\n\n");
        /* Ce texte indique qu'une traduction reste à faire. */
        g_string_append_printf(preview_text,
            "Original:\n%s\n\nTraduction:\n[Traduction en attente]", sentence);
    }

    GtkTextBuffer *preview_buffer =
        gtk_text_view_get_buffer(state->preview);
    gtk_text_buffer_set_text(preview_buffer, preview_text->str, -1);

    gchar *status_text = g_strdup_printf("%u phrase%s terminée%s",
        sentences->len, sentences->len > 1 ? "s" : "",
        sentences->len > 1 ? "s" : "");
    gtk_label_set_text(state->status, status_text);

    g_free(status_text);
    g_string_free(preview_text, TRUE);
    g_ptr_array_unref(sentences);
    g_free(source_text);
}

static GtkWidget *
create_text_area(gboolean editable, const gchar *placeholder)
{
    /* Une seule fonction configure les deux zones de texte de l'écran. */
    GtkWidget *view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), editable);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), editable);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 12);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 12);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(view), 12);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(view), 12);
    if (placeholder != NULL)
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(view)),
            placeholder, -1);
    return view;
}

static GtkWidget *
create_scrolled_text(GtkWidget *view)
{
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), view);
    gtk_widget_set_hexpand(scroll, TRUE);
    gtk_widget_set_vexpand(scroll, TRUE);
    return scroll;
}

GtkWidget *
editor_new(void)
{
    /* La fenêtre principale reçoit cette boîte comme interface complète. */
    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(root, 20);
    gtk_widget_set_margin_bottom(root, 16);
    gtk_widget_set_margin_start(root, 20);
    gtk_widget_set_margin_end(root, 20);

    GtkWidget *heading = gtk_label_new("AutoTranslate — Français / Vietnamien");
    gtk_widget_add_css_class(heading, "title-2");
    gtk_widget_set_halign(heading, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(root), heading);

    GtkWidget *language_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    const gchar *source_languages[] = {"Français", "Vietnamien", NULL};
    const gchar *target_languages[] = {"Vietnamien", "Français", NULL};
    GtkWidget *source = gtk_drop_down_new_from_strings(source_languages);
    GtkWidget *target = gtk_drop_down_new_from_strings(target_languages);
    gtk_widget_set_hexpand(source, TRUE);
    gtk_widget_set_hexpand(target, TRUE);
    gtk_box_append(GTK_BOX(language_row), source);
    gtk_box_append(GTK_BOX(language_row), gtk_label_new("→"));
    gtk_box_append(GTK_BOX(language_row), target);
    gtk_box_append(GTK_BOX(root), language_row);

    GtkWidget *source_label = gtk_label_new("Texte original");
    gtk_widget_set_halign(source_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(root), source_label);

    GtkWidget *source_view = create_text_area(TRUE, NULL);
    gtk_box_append(GTK_BOX(root), create_scrolled_text(source_view));

    GtkWidget *preview_label = gtk_label_new("Aperçu des phrases terminées");
    gtk_widget_set_halign(preview_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(root), preview_label);

    GtkWidget *preview_view = create_text_area(FALSE, NULL);
    gtk_box_append(GTK_BOX(root), create_scrolled_text(preview_view));

    GtkWidget *status_label = gtk_label_new("0 phrase terminée");
    gtk_widget_set_halign(status_label, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(root), status_label);

    /* L'état reste valide aussi longtemps que la boîte existe. */
    EditorState *state = g_new0(EditorState, 1);
    state->preview = GTK_TEXT_VIEW(preview_view);
    state->status = GTK_LABEL(status_label);
    g_object_set_data_full(G_OBJECT(root), "autotranslate-editor-state",
        state, g_free);
    g_signal_connect(gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)),
        "changed", G_CALLBACK(on_source_changed), state);

    return root;
}
