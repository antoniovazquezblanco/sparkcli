/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_PROGRESS_
#define _H_PROGRESS_

#include "api.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    size_t total;
    size_t current;
    char *label;
} scli_prg_t;

SCLI_API scli_prg_t *scli_prg_new(size_t total, char *label);
SCLI_API void scli_prg_free(scli_prg_t *progress);

SCLI_API void scli_prg_update(scli_prg_t *progress, size_t current);
SCLI_API void scli_prg_finish(scli_prg_t *progress);

#ifdef __cplusplus
}
#endif

#endif /* _H_PROGRESS_ */
