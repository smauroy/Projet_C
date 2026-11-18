#ifndef AUTOTRANSLATE_SENTENCE_PARSER_H
#define AUTOTRANSLATE_SENTENCE_PARSER_H

#include <glib.h>

/*
 * Renvoie les phrases terminées par un point dans un tableau.
 * Le tableau contient aussi les textes des phrases : utilisez
 * g_ptr_array_unref() pour tout libérer après usage.
 */
GPtrArray *sentence_parser_extract_completed(const gchar *text);

#endif
