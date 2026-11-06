/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <sparkcli.h>

/**
 * scli_tbl_new() and scli_tbl_add_row() promise to duplicate what they are
 * given, so a caller may hand over a buffer it is about to reuse. Overwrite
 * both buffers after handing them over and check the table kept its own copy.
 */
static void test_tbl_copies_caller_strings(void **state)
{
    (void)state;

    char header[] = "Component";
    char cell[] = "Renderer";
    const char *const headers[] = {header};
    const char *const cells[] = {cell};

    scli_tbl_t *table = scli_tbl_new(1, headers);
    assert_non_null(table);
    assert_true(scli_tbl_add_row(table, cells));

    /* Scribble over what the caller passed in. */
    header[0] = 'X';
    cell[0] = 'Y';

    assert_int_equal(table->column_count, 1);
    assert_int_equal(table->row_count, 1);
    assert_string_equal(table->headers[0], "Component");
    assert_string_equal(table->cells[0], "Renderer");

    scli_tbl_free(&table);
    assert_null(table);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_tbl_copies_caller_strings),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
