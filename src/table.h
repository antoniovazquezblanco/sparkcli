/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_TABLE_
#define _H_TABLE_

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    size_t column_count;
    char **headers;
    size_t row_count;
    size_t row_capacity;
    char **cells;
} scli_tbl_t;

/**
 * Allocate a table with @column_count columns.
 * Header strings are duplicated, so callers can pass temporary values.
 */
scli_tbl_t *scli_tbl_new(size_t column_count, char *headers[]);

/**
 * Free the table and all strings owned by it.
 */
void scli_tbl_free(scli_tbl_t **table);

/**
 * Duplicate @cells and append them as a new row. NULL cells map to empty strings.
 */
bool scli_tbl_add_row(scli_tbl_t *table, char *cells[]);

void scli_tbl_render(scli_tbl_t *table);

#endif /* _H_TABLE_ */
