#include <glib.h>

#include "../src/core/sentence_parser.h"

static void
assert_sentences(const gchar *input, const gchar *const *expected,
    guint expected_count)
{
    GPtrArray *sentences = sentence_parser_extract_completed(input);
    g_assert_cmpuint(sentences->len, ==, expected_count);
    for (guint i = 0; i < expected_count; i++)
        g_assert_cmpstr(g_ptr_array_index(sentences, i), ==, expected[i]);
    g_ptr_array_unref(sentences);
}

static void
test_empty_input(void)
{
    const gchar *expected[] = {NULL};
    assert_sentences("", expected, 0);
}

static void
test_unfinished_sentence(void)
{
    const gchar *expected[] = {NULL};
    assert_sentences("Je suis étudiant", expected, 0);
}

static void
test_completed_and_multiple_sentences(void)
{
    const gchar *one[] = {"Bonjour."};
    const gchar *many[] = {"Bonjour.", "Je suis étudiant."};
    assert_sentences("Bonjour.", one, G_N_ELEMENTS(one));
    assert_sentences("Bonjour. Je suis étudiant.", many, G_N_ELEMENTS(many));
}

static void
test_edit_and_delete_period(void)
{
    const gchar *completed[] = {"Bonjour."};
    const gchar *unfinished[] = {NULL};
    assert_sentences("Bonjour.", completed, G_N_ELEMENTS(completed));
    assert_sentences("Bonjour", unfinished, 0);
}

static void
test_pasted_sentences(void)
{
    const gchar *expected[] = {"Bonjour.", "Xin chào.", "Tôi đang học C."};
    assert_sentences("Bonjour. Xin chào. Tôi đang học C.", expected,
        G_N_ELEMENTS(expected));
}

static void
test_utf8(void)
{
    const gchar *expected[] = {
        "Je suis étudiant à ESGI.", "Xin chào, tôi là sinh viên."};
    assert_sentences("Je suis étudiant à ESGI. Xin chào, tôi là sinh viên.",
        expected, G_N_ELEMENTS(expected));
}

static void
test_decimal_number(void)
{
    const gchar *expected[] = {"La valeur est 3.14."};
    assert_sentences("La valeur est 3.14.", expected, G_N_ELEMENTS(expected));
}

int
main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/parser/empty", test_empty_input);
    g_test_add_func("/parser/unfinished", test_unfinished_sentence);
    g_test_add_func("/parser/completed-and-multiple",
        test_completed_and_multiple_sentences);
    g_test_add_func("/parser/edit-delete", test_edit_and_delete_period);
    g_test_add_func("/parser/paste", test_pasted_sentences);
    g_test_add_func("/parser/utf8", test_utf8);
    g_test_add_func("/parser/decimal", test_decimal_number);
    return g_test_run();
}
