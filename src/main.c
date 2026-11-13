#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer data)
{
    (void)data;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "AutoTranslate");
    gtk_window_set_default_size(GTK_WINDOW(window), 850, 550);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_widget_set_margin_bottom(box, 20);

    GtkWidget *title = gtk_label_new("AutoTranslate — Français / Vietnamien");
    GtkWidget *scroll = gtk_scrolled_window_new();
    GtkWidget *editor = gtk_text_view_new();
    GtkWidget *result = gtk_label_new("La traduction apparaîtra ici.");

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(editor), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), editor);
    gtk_widget_set_vexpand(scroll, TRUE);

    gtk_box_append(GTK_BOX(box), title);
    gtk_box_append(GTK_BOX(box), scroll);
    gtk_box_append(GTK_BOX(box), result);

    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new(
        "fr.autotranslate.editor",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
