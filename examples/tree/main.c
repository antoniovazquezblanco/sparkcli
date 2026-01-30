/*
* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Kevin Leon
*/

#include <sparkcli.h>
#include <stdio.h>
#include <stdlib.h>

const char *BANNER =
    "  ___                _    ___ _    ___ \n"
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n"
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n"
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n"
    "     |_|                               \n";


int main(void) {
 printf(BANNER);
 printf("Tree helper example for SparkCLI\n\n");

 scli_tree_node_t* root = scli_tree_new("Satellite");

 scli_tree_node_t *child_aocs = scli_tree_new("AOCS");
 scli_tree_node_t *child_cdh = scli_tree_new("CD&H");
 if (!scli_tree_add_child(root, child_aocs)) {
  printf("Error!\n");
  return 1;
 }
 if (!scli_tree_add_child(root, child_cdh)) {
  printf("Error!\n");
  return 1;
 }

 scli_tree_add_child(child_cdh, scli_tree_new("OBC"));
 scli_tree_add_child(child_cdh, scli_tree_new("Memory"));

 printf("Example output:\n");
 scli_tree_render(root);
 scli_tree_node_free(root);
 return 0;
}