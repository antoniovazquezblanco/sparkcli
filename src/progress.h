/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_PROGRESS_
#define _H_PROGRESS_

#include <stddef.h>

typedef struct
{
    size_t total;
    size_t current;
    char *label;
} scli_prg_t;

scli_prg_t *scli_prg_new(size_t total, char *label);
void scli_prg_free(scli_prg_t *progress);

void scli_prg_update(scli_prg_t *progress, size_t current);
void scli_prg_finish(scli_prg_t *progress);

#endif /* _H_PROGRESS_ */
