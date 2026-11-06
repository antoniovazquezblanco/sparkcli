/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

/*
 * The public headers must be usable from C++. Without the extern "C" guards
 * these declarations would get C++ linkage and fail to resolve against the C
 * library, so linking this file is the check.
 */

#include <sparkcli.h>

#include <cstddef>
#include <cstdint>

int main()
{
    /* Touch a symbol from every public header so each one has to link. */
    (void)scli_ses_interactive();
    (void)scli_ses_color();
    (void)scli_color_system();
    scli_color_reset();
    scli_log_info("%s", "from C++");

    scli_tree_node_t *tree = scli_tree_new("root");
    scli_tree_add_child(tree, scli_tree_new("child"));
    scli_tree_node_free(tree);

    const char *const headers[] = {"a"};
    scli_tbl_t *table = scli_tbl_new(1, headers);
    scli_tbl_align(table, 0, SCLI_TBL_ALIGN_RIGHT);
    scli_tbl_free(&table);

    scli_prg_t *progress = scli_prg_new(1, "p");
    scli_prg_finish(progress);
    scli_prg_free(progress);

    const std::uint8_t data[] = {0x53, 0x70};
    scli_hex16(data, sizeof(data));
    return 0;
}
