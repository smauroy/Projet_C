#include "sentence_parser.h"

static gchar *
trim_sentence(const gchar *start, const gchar *end)
{
    while (start < end) {
        gunichar ch = g_utf8_get_char(start);
        if (!g_unichar_isspace(ch))
            break;
        start = g_utf8_next_char(start);
    }

    while (end > start) {
        const gchar *previous = g_utf8_find_prev_char(start, end);
        if (previous == NULL || !g_unichar_isspace(g_utf8_get_char(previous)))
            break;
        end = previous;
    }

    return g_strndup(start, (gsize)(end - start));
}

GPtrArray *
sentence_parser_extract_completed(const gchar *text)
{
    GPtrArray *sentences = g_ptr_array_new_with_free_func(g_free);
    if (text == NULL || !g_utf8_validate(text, -1, NULL) || *text == '\0')
        return sentences;

    const gchar *sentence_start = text;
    const gchar *cursor = text;
    while (*cursor != '\0') {
        if (*cursor == '.') {
            gboolean decimal_point = cursor > text && cursor[1] != '\0' &&
                g_ascii_isdigit(cursor[-1]) && g_ascii_isdigit(cursor[1]);
            if (!decimal_point) {
                gchar *sentence = trim_sentence(sentence_start, cursor + 1);
                if (*sentence != '\0')
                    g_ptr_array_add(sentences, sentence);
                else
                    g_free(sentence);
                sentence_start = cursor + 1;
            }
        }
        cursor = g_utf8_next_char(cursor);
    }

    return sentences;
}
