#ifndef AUTOTRANSLATE_SENTENCE_PARSER_H
#define AUTOTRANSLATE_SENTENCE_PARSER_H

#include <glib.h>

/*
 * Returns completed sentences, including their terminating periods, as an
 * owning GPtrArray of gchar*. The caller frees the array with g_ptr_array_unref.
 */
GPtrArray *sentence_parser_extract_completed(const gchar *text);

#endif
