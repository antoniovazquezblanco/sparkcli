/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <sparkcli.h>

/**
 * A program reporting SparkCLI in its own --version output needs a version
 * that is actually populated.
 */
static void test_version_string_is_populated(void **state)
{
    (void)state;

    assert_non_null(scli_version_string());
    assert_true(strlen(scli_version_string()) > 0);
}

/**
 * The string always opens with the release components, whatever build
 * metadata follows it.
 */
static void test_string_starts_with_the_components(void **state)
{
    (void)state;

    char expected[32];
    snprintf(expected, sizeof(expected), "%u.%u.%u",
             scli_version_major(), scli_version_minor(), scli_version_patch());

    assert_memory_equal(scli_version_string(), expected, strlen(expected));
}

/**
 * The encoded number orders the way the components do, so two releases
 * compare with a single operator.
 */
static void test_number_encodes_the_components(void **state)
{
    (void)state;

    uint32_t expected = ((uint32_t)scli_version_major() << 16) |
                        ((uint32_t)scli_version_minor() << 8) |
                        (uint32_t)scli_version_patch();
    assert_int_equal(scli_version_number(), expected);

    /* Components are byte sized, so none of them may bleed into the next. */
    assert_true(scli_version_minor() <= 0xff);
    assert_true(scli_version_patch() <= 0xff);
}

/**
 * Commit distance and hash describe where past a release the build came from.
 * A tagged build reports no commits; anything past the tag records both.
 */
static void test_commit_distance_and_hash(void **state)
{
    (void)state;

    assert_non_null(scli_version_commit());

    if (scli_version_commits() > 0)
    {
        /* Past a tag, the hash has to be there to identify the commit. */
        assert_true(strlen(scli_version_commit()) > 0);

        /* and it has to appear in the full string, prefixed with g. */
        char needle[32];
        snprintf(needle, sizeof(needle), "g%s", scli_version_commit());
        assert_non_null(strstr(scli_version_string(), needle));
    }
}

/**
 * The dirty flag and the string have to agree: a release tarball that claims
 * to be clean must not carry the marker.
 */
static void test_dirty_flag_matches_the_string(void **state)
{
    (void)state;

    bool marked = strstr(scli_version_string(), "dirty") != NULL;
    assert_int_equal(scli_version_dirty() ? 1 : 0, marked ? 1 : 0);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_version_string_is_populated),
        cmocka_unit_test(test_string_starts_with_the_components),
        cmocka_unit_test(test_number_encodes_the_components),
        cmocka_unit_test(test_commit_distance_and_hash),
        cmocka_unit_test(test_dirty_flag_matches_the_string),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
