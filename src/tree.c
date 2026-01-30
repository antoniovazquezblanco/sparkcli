/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Kevin Leon
 */
#include "tree.h"

#include "platform/console.h"
#include "utils/str.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Table border style definition.
 */
typedef struct
{
    const char *branch;
    const char *last;
    const char *vertical;
    const char *space;
} _tree_level_style_t;

/**
 * A simple ASCII tree style.
 */
static const _tree_level_style_t _tree_style_ascii = {
    .branch = "|-- ",
    .last = "`-- ",
    .vertical = "|   ",
    .space = "    ",
};

/**
 * A simple UTF-8 tree style.
 */
static const _tree_level_style_t _tree_style_utf8 = {
    .branch = "├── ",
    .last = "└── ",
    .vertical = "│   ",
    .space = "    ",
};

static void _render_tree(const scli_tree_node_t *node, const char *prefix, int is_last, const _tree_level_style_t *style)
{
    if (!node)
        return;

    printf("%s", prefix);
    printf("%s", is_last ? style->last : style->branch);
    printf("%s\n", node->name);

    for (size_t i = 0; i < node->child_count; i++)
    {
        const char *extra = is_last ? style->space : style->vertical;

        size_t len = strlen(prefix) + strlen(extra) + 1;
        char *new_prefix = malloc(len);
        if (!new_prefix)
            return;

        snprintf(new_prefix, len, "%s%s", prefix, extra);

        _render_tree(node->children[i], new_prefix, i == node->child_count - 1, style);

        free(new_prefix);
    }
}

scli_tree_node_t *scli_tree_new(const char *name)
{
    scli_tree_node_t *node = malloc(sizeof(scli_tree_node_t));
    if (!node)
        return NULL;

    node->name = strdup(str_safe(name));
    node->children = NULL;
    node->child_count = 0;
    return node;
}

bool scli_tree_add_child(scli_tree_node_t *parent, scli_tree_node_t *child)
{
    if (!parent || !child)
        return false;

    scli_tree_node_t **new_children = realloc(
        parent->children,
        sizeof(*parent->children) * (parent->child_count + 1));

    if (!new_children)
        return false;

    parent->children = new_children;

    parent->children[parent->child_count] = child;
    parent->child_count++;

    return true;
}

void scli_tree_node_free(scli_tree_node_t *node)
{
    if (!node)
        return;
    for (size_t i = 0; i < node->child_count; i++)
        scli_tree_node_free(node->children[i]);
    free(node->children);
    free(node->name);
}

void scli_tree_render(const scli_tree_node_t *node)
{
    static const _tree_level_style_t *style = &_tree_style_ascii;
    if (console_set_utf8_output())
        style = &_tree_style_utf8;

    printf("%s\n", node->name);
    for (size_t i = 0; i < node->child_count; i++)
        _render_tree(node->children[i], "", i == node->child_count - 1, style);
}
