/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <sparkcli.h>

static void test_new_node_has_no_children(void **state)
{
    (void)state;

    scli_tree_node_t *root = scli_tree_new("root");
    assert_non_null(root);
    assert_string_equal(root->name, "root");
    assert_int_equal(root->child_count, 0);
    assert_null(root->children);

    scli_tree_node_free(root);
}

static void test_children_are_attached(void **state)
{
    (void)state;

    scli_tree_node_t *root = scli_tree_new("root");
    scli_tree_node_t *first = scli_tree_new("first");
    scli_tree_node_t *second = scli_tree_new("second");

    assert_true(scli_tree_add_child(root, first));
    assert_true(scli_tree_add_child(root, second));
    assert_true(scli_tree_add_child(first, scli_tree_new("nested")));

    assert_int_equal(root->child_count, 2);
    assert_ptr_equal(root->children[0], first);
    assert_ptr_equal(root->children[1], second);
    assert_int_equal(first->child_count, 1);
    assert_string_equal(first->children[0]->name, "nested");

    scli_tree_node_free(root);
}

static void test_add_child_rejects_null(void **state)
{
    (void)state;

    scli_tree_node_t *root = scli_tree_new("root");

    assert_false(scli_tree_add_child(root, NULL));
    assert_false(scli_tree_add_child(NULL, root));
    assert_int_equal(root->child_count, 0);

    scli_tree_node_free(root);
}

/**
 * scli_tree_node_free() must release the nodes themselves, not only their
 * names and child arrays. Build a tree deep enough that a leaked node is
 * worth catching, then free it; run under -Db_sanitize=address to see a
 * regression here reported as a leak.
 */
static void test_whole_tree_is_released(void **state)
{
    (void)state;

    scli_tree_node_t *root = scli_tree_new("root");
    for (int branch = 0; branch < 4; ++branch)
    {
        scli_tree_node_t *child = scli_tree_new("child");
        assert_true(scli_tree_add_child(root, child));
        for (int leaf = 0; leaf < 4; ++leaf)
            assert_true(scli_tree_add_child(child, scli_tree_new("leaf")));
    }

    assert_int_equal(root->child_count, 4);
    scli_tree_node_free(root);
}

static void test_free_and_render_tolerate_null(void **state)
{
    (void)state;

    scli_tree_node_free(NULL);
    scli_tree_render(NULL);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_node_has_no_children),
        cmocka_unit_test(test_children_are_attached),
        cmocka_unit_test(test_add_child_rejects_null),
        cmocka_unit_test(test_whole_tree_is_released),
        cmocka_unit_test(test_free_and_render_tolerate_null),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
