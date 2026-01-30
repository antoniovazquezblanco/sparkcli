/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Kevin Leonn
 */

#ifndef _H_TREE_
#define _H_TREE_

#include <stdbool.h>
#include <stddef.h>

typedef struct tree_node {
  char *name;
  struct tree_node **children;
  size_t child_count;
} scli_tree_node_t;

/**
 * Allocates a tree with root node. REMEBER FREE the node
 *
 * @param name The name of the root node.
 * @return The node struct. Returns NULL in case of error.
*/
scli_tree_node_t *scli_tree_new(const char *name);
/**
 * Add child to node
 *
 * @param parent The parent node
 * @param child The child node to add
 * @return The true if OK.
*/
bool scli_tree_add_child(scli_tree_node_t *parent, scli_tree_node_t *child);
/**
 * Free memory allocation of the tree.
 *
 * @param tree The node.
*/
void scli_tree_node_free(scli_tree_node_t *node);

/**
 * Render the tree
 *
 * @param tree The node.
*/
void scli_tree_render(const scli_tree_node_t *node);
#endif /* _H_TREE_ */
