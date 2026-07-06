/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio V\u00e1zquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "progress.h"

#include "platform/console.h"
#include "utils/str.h"
#include "utils/math.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Progress bar rendering style.
 *
 * A style describes how to draw the bar cell by cell. Styles that provide
 * partial glyphs allow sub-cell resolution, giving smoother animations on
 * terminals that support the required Unicode characters.
 */
typedef struct
{
    const char *left;            /* Opening bracket of the bar. */
    const char *right;           /* Closing bracket of the bar. */
    const char *full;            /* A completely filled cell. */
    const char *empty;           /* A completely empty cell. */
    const char *const *partials; /* Partial-fill glyphs, from least to most filled. */
    size_t partial_count;        /* Number of entries in @partials. */
} _prg_style_t;

/**
 * A simple ASCII style that renders on any terminal.
 */
static const _prg_style_t _prg_style_ascii = {
    .left = "[",
    .right = "]",
    .full = "#",
    .empty = "-",
    .partials = NULL,
    .partial_count = 0,
};

/**
 * Eighth-block glyphs used to fill a cell partially (1/8 .. 7/8).
 */
static const char *const _prg_utf8_partials[] = {
    "\u258f", /* left one eighth block   (1/8) */
    "\u258e", /* left one quarter block  (2/8) */
    "\u258d", /* left three eighths block (3/8) */
    "\u258c", /* left half block         (4/8) */
    "\u258b", /* left five eighths block (5/8) */
    "\u258a", /* left three quarters block (6/8) */
    "\u2589", /* left seven eighths block (7/8) */
};

/**
 * A UTF-8 style using block-drawing characters for smooth, fancy animations.
 */
static const _prg_style_t _prg_style_utf8 = {
    .left = "",
    .right = "",
    .full = "\u2588",  /* full block */
    .empty = "\u2591", /* light shade */
    .partials = _prg_utf8_partials,
    .partial_count = sizeof(_prg_utf8_partials) / sizeof(_prg_utf8_partials[0]),
};

/**
 * Select the active style once, preferring the fancier UTF-8 style whenever the
 * console can render it. The result is cached for subsequent redraws.
 */
static const _prg_style_t *_prg_active_style(void)
{
    static bool initialized = false;
    static const _prg_style_t *style = &_prg_style_ascii;
    if (!initialized)
    {
        initialized = true;
        if (console_set_utf8_output())
            style = &_prg_style_utf8;
    }
    return style;
}

/**
 * Helper function to draw the progress bar.
 */
static void _scli_prg_draw(const scli_prg_t *progress, bool finished)
{
    if (!progress)
        return;

    const _prg_style_t *style = _prg_active_style();

    size_t total = progress->total ? progress->total : 1;
    size_t current = min(progress->current, total);

    const size_t width = 30;

    // Work in sub-cell units so styles with partial glyphs animate smoothly.
    size_t substeps = style->partial_count + 1;
    size_t filled_units = (current * width * substeps) / total;
    size_t full_cells = min(filled_units / substeps, width);
    size_t partial_index = filled_units % substeps;
    int percent = (int)((current * 100) / total);

    printf("\r%s %s", str_safe(progress->label), style->left);

    size_t drawn = 0;
    for (; drawn < full_cells; ++drawn)
        fputs(style->full, stdout);

    // Render a single partial cell for the leftover fraction, if any.
    if (partial_index > 0 && drawn < width)
    {
        fputs(style->partials[partial_index - 1], stdout);
        ++drawn;
    }

    for (; drawn < width; ++drawn)
        fputs(style->empty, stdout);

    printf("%s %3d%%", style->right, percent);

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
