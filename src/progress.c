/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "progress.h"

#include "utils/str.h"
#include "utils/math.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Helper function to draw the progress bar.
 */
static void _scli_prg_draw(const scli_prg_t *progress, bool finished)
{
    if (!progress)
        return;

    size_t total = progress->total ? progress->total : 1;
    size_t current = min(progress->current, total);

    const size_t width = 30;
    size_t filled = (current * width) / total;
    size_t empty = width - filled;
    int percent = (int)((current * 100) / total);

    printf("\r%s [", str_safe(progress->label));
    for (size_t index = 0; index < filled; ++index)
        putchar('#');
    for (size_t index = 0; index < empty; ++index)
        putchar('-');
    printf("] %3d%%", percent);

    if (finished)
        putchar('\n');

    fflush(stdout);
}


scli_prg_t *scli_prg_new(size_t total, char *label)
{
    scli_prg_t *progress = malloc(sizeof(scli_prg_t));
    if (!progress)
        return NULL;

    progress->total = total;
    progress->current = 0;
    progress->label = NULL;
    progress->label = strdup(str_safe(label));
    _scli_prg_draw(progress, false);

    return progress;
}

void scli_prg_free(scli_prg_t *progress)
{
    if (!progress)
        return;

    free(progress->label);
    free(progress);
}

void scli_prg_update(scli_prg_t *progress, size_t current)
{
    if (!progress)
        return;

    progress->current = current;
    _scli_prg_draw(progress, false);
}

void scli_prg_finish(scli_prg_t *progress)
{
    if (!progress)
        return;

    progress->current = progress->total;
    _scli_prg_draw(progress, true);
}

