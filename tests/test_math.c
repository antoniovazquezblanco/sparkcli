/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "utils/math.h"

/**
 * Operators binding looser than '<' must survive the expansion. Unparenthesised,
 * min(8, 4 | 2) expands to (8 < 4 | 2 ? ...), which compares 8 against 4 and
 * then ORs the result with 2, yielding 8 instead of 6.
 */
static void test_min_max_bind_loose_operators(void **state)
{
    (void)state;

    assert_int_equal(min(8, 4 | 2), 6);
    assert_int_equal(max(1, 4 | 2), 6);
    assert_int_equal(min(1, 0 | 0), 0);
    assert_int_equal(max(5, 1 ^ 3), 5);
    assert_int_equal(min(3, 1 == 1), 1);
}

static void test_min_max_plain_values(void **state)
{
    (void)state;

    assert_int_equal(min(1, 2), 1);
    assert_int_equal(max(1, 2), 2);
    assert_int_equal(min(7, 7), 7);
    assert_int_equal(max(-3, -9), -3);
    assert_int_equal(min(2 + 3, 4), 4);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_min_max_bind_loose_operators),
        cmocka_unit_test(test_min_max_plain_values),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
