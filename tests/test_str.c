/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <sparkcli.h>

#include "utils/str.h"

/**
 * str_safe() must guard a conditional expression as well as a plain pointer.
 * Unparenthesised, str_safe(c ? c[i] : "") expands to a chain that hands back
 * c[i] unguarded, so a NULL cell escapes the macro.
 */
static void test_str_safe_guards_a_conditional(void **state)
{
    (void)state;

    char *values[] = {"value"};
    char *holes[] = {NULL};
    char **present = values;
    char **missing = holes;
    char **none = NULL;

    assert_string_equal(str_safe(present ? present[0] : ""), "value");
    assert_string_equal(str_safe(missing ? missing[0] : ""), "");
    assert_string_equal(str_safe(none ? none[0] : ""), "");
}

/**
 * scli_tbl_add_row() documents that NULL cells map to empty strings. Before
 * the macro was fixed this dereferenced NULL inside the string duplication.
 */
static void test_tbl_accepts_null_cells(void **state)
{
    (void)state;

    char *headers[] = {"a", "b"};
    char *cells[] = {"set", NULL};

    scli_tbl_t *table = scli_tbl_new(2, headers);
    assert_non_null(table);

    assert_true(scli_tbl_add_row(table, cells));
    assert_true(scli_tbl_add_row(table, NULL));

    assert_int_equal(table->row_count, 2);
    assert_string_equal(table->cells[0], "set");
    assert_string_equal(table->cells[1], "");
    assert_string_equal(table->cells[2], "");
    assert_string_equal(table->cells[3], "");

    scli_tbl_free(&table);
    assert_null(table);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_str_safe_guards_a_conditional),
        cmocka_unit_test(test_tbl_accepts_null_cells),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
