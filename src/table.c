/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "table.h"

#include "utils/str.h"
#include "utils/math.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/**
 * Helper function to print a border row of a table.
 */
static void _print_row_border(size_t column_count, size_t widths[])
{
    putchar('+');
    for (size_t column = 0; column < column_count; ++column)
    {
        for (size_t count = 0; count < widths[column] + 2; ++count)
            putchar('-');
        putchar('+');
    }
    putchar('\n');
}

/**
 * Helper function to print a table row.
 */
static void _print_row(size_t column_count, size_t widths[], char *cells[])
{
    if (!cells)
        return;
    putchar('|');
    for (size_t column = 0; column < column_count; ++column)
        printf(" %-*s |", (int)widths[column], str_safe(cells[column]));
    putchar('\n');
}

void scli_tbl_render(const scli_tbl_t *table)
{
    if (!table || table->column_count == 0)
        return;

    // Calculate column widths
    size_t widths[table->column_count] = {};
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
    _print_row_border(table->column_count, widths);
    _print_row(table->column_count, widths, table->headers);
    _print_row_border(table->column_count, widths);
    for (size_t row = 0; row < table->row_count; ++row)
    {
        char **row_cells = _table_row(table, row);
        _print_row(table->column_count, widths, row_cells);
    }
    _print_row_border(table->column_count, widths);
}
