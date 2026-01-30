/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "table.h"

#include "platform/console.h"
#include "utils/str.h"
#include "utils/math.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Table border style definition.
 */
typedef struct
{
    const char *horizontal;
    const char *vertical;
    const char *top_left;
    const char *top_joint;
    const char *top_right;
    const char *mid_left;
    const char *mid_joint;
    const char *mid_right;
    const char *bot_left;
    const char *bot_joint;
    const char *bot_right;
} _tbl_border_style_t;

typedef enum
{
    _TBL_BORDER_SECTION_TOP,
    _TBL_BORDER_SECTION_MIDDLE,
    _TBL_BORDER_SECTION_BOTTOM,
} _tbl_border_section_t;

/**
 * A simple ASCII table style.
 */
static const _tbl_border_style_t _tbl_style_ascii = {
    .horizontal = "-",
    .vertical = "|",
    .top_left = "+",
    .top_joint = "+",
    .top_right = "+",
    .mid_left = "+",
    .mid_joint = "+",
    .mid_right = "+",
    .bot_left = "+",
    .bot_joint = "+",
    .bot_right = "+",
};

/**
 * A UTF-8 table style with full box drawing characters.
 */
static const _tbl_border_style_t _tbl_style_utf8 = {
    .horizontal = "\u2500",
    .vertical = "\u2502",
    .top_left = "\u250c",
    .top_joint = "\u252c",
    .top_right = "\u2510",
    .mid_left = "\u251c",
    .mid_joint = "\u253c",
    .mid_right = "\u2524",
    .bot_left = "\u2514",
    .bot_joint = "\u2534",
    .bot_right = "\u2518",
};

/**
 * Free an str array.
 */
static void _strarrfree(char *strarr[], size_t element_count)
{
    if (!strarr)
        return;
    for (size_t i = 0; i < element_count; ++i)
        free(strarr[i]);
    free(strarr);
}

/**
 * Duplicate (make a copy) of an str array.
 */
static char **_strarrdup(char *strarr[], size_t element_count)
{
    if (!strarr)
        return NULL;
    char **copy = malloc(sizeof(char *) * element_count);
    if (!copy)
        return NULL;
    for (size_t i = 0; i < element_count; ++i)
    {
        copy[i] = strdup(str_safe(strarr[i]));
        if (!copy[i])
        {
            _strarrfree(copy, element_count);
            return NULL;
        }
    }
    return copy;
}

scli_tbl_t *scli_tbl_new(size_t column_count, char *headers[])
{
    scli_tbl_t *table = malloc(sizeof(scli_tbl_t));
    if (!table)
        return NULL;
    table->column_count = column_count;
    table->headers = _strarrdup(headers, column_count);
    if (headers && !table->headers)
    {
        free(table);
        return NULL;
    }
    table->row_count = 0;
    table->row_capacity = 0;
    table->cells = NULL;

    return table;
}

void scli_tbl_free(scli_tbl_t **table_ptr)
{
    if (!table_ptr)
        return;
    scli_tbl_t *table = *table_ptr;
    size_t total_cells = table->row_count * table->column_count;
    _strarrfree(table->cells, total_cells);
    table->cells = NULL;
    table->row_capacity = 0;
    table->row_count = 0;
    _strarrfree(table->headers, table->column_count);
    free(table);
    *table_ptr = NULL;
}

bool scli_tbl_add_row(scli_tbl_t *table, char *cells[])
{
    if (!table || table->column_count == 0)
        return false;

    if (table->row_count >= table->row_capacity)
    {
        size_t new_capacity = table->row_capacity ? table->row_capacity * 2 : 4;
        size_t new_size = new_capacity * table->column_count;
        char **new_cells = realloc(table->cells, sizeof(*table->cells) * new_size);
        if (!new_cells)
            return false;
        table->cells = new_cells;
        table->row_capacity = new_capacity;
    }

    char **target = table->cells + table->row_count * table->column_count;
    for (size_t column = 0; column < table->column_count; ++column)
    {
        char *value = strdup(str_safe(cells ? cells[column] : ""));
        if (!value)
        {
            for (size_t cleanup = 0; cleanup < column; ++cleanup)
                free(target[cleanup]);
            return false;
        }
        target[column] = value;
    }

    ++table->row_count;
    return true;
}

/**
 * Helper function to get a particular table row.
 */
static char **_table_row(const scli_tbl_t *table, size_t row)
{
    if (!table || !table->cells)
        return NULL;
    return (table->cells + (row * table->column_count));
}

static void _print_border(const _tbl_border_style_t *style, _tbl_border_section_t section, size_t column_count, const size_t widths[])
{
    const char *left = style->mid_left;
    const char *joint = style->mid_joint;
    const char *right = style->mid_right;
    if (section == _TBL_BORDER_SECTION_TOP)
    {
        left = style->top_left;
        joint = style->top_joint;
        right = style->top_right;
    }
    else if (section == _TBL_BORDER_SECTION_BOTTOM)
    {
        left = style->bot_left;
        joint = style->bot_joint;
        right = style->bot_right;
    }
    fputs(left, stdout);
    for (size_t column = 0; column < column_count; ++column)
    {
        for (size_t count = 0; count < widths[column] + 2; ++count)
            fputs(style->horizontal, stdout);
        fputs(column + 1 == column_count ? right : joint, stdout);
    }
    putchar('\n');
}

static void _print_row(const _tbl_border_style_t *style, size_t column_count, const size_t widths[], char *cells[])
{
    if (!cells)
        return;
    fputs(style->vertical, stdout);
    for (size_t column = 0; column < column_count; ++column)
    {
        const char *value = str_safe(cells[column]);
        printf(" %-*s ", (int)widths[column], value);
        fputs(style->vertical, stdout);
    }
    putchar('\n');
}

static void _tbl_render(scli_tbl_t *table, const _tbl_border_style_t *style)
{
    if (!table || table->column_count == 0)
        return;

    // Calculate column widths
    size_t *widths = calloc(table->column_count, sizeof(*widths));
    if (!widths)
        return;
    if (table->headers != NULL)
        for (size_t column = 0; column < table->column_count; ++column)
            widths[column] = strlen(str_safe(table->headers[column]));
    for (size_t row = 0; row < table->row_count; ++row)
    {
        char **row_cells = _table_row(table, row);
        if (row_cells != NULL)
            for (size_t column = 0; column < table->column_count; ++column)
            {
                size_t current_length = strlen(str_safe(row_cells[column]));
                widths[column] = max(widths[column], current_length);
            }
    }

    // Print the table...
    _print_border(style, _TBL_BORDER_SECTION_TOP, table->column_count, widths);
    _print_row(style, table->column_count, widths, table->headers);
    _print_border(style, _TBL_BORDER_SECTION_MIDDLE, table->column_count, widths);
    for (size_t row = 0; row < table->row_count; ++row)
    {
        char **row_cells = _table_row(table, row);
        _print_row(style, table->column_count, widths, row_cells);
    }
    _print_border(style, _TBL_BORDER_SECTION_BOTTOM, table->column_count, widths);
    free(widths);
}

void scli_tbl_render(scli_tbl_t *table)
{
    static bool initialized = false;
    static const _tbl_border_style_t *style = &_tbl_style_ascii;
    if (!initialized)
    {
        initialized = true;
        if (console_set_utf8_output())
            style = &_tbl_style_utf8;
    }
    _tbl_render(table, style);
}
