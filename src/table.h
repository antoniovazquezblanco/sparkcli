/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_TABLE_
#define _H_TABLE_

#include <stdbool.h>
#include <stddef.h>

/**
 * Horizontal alignment of a column's contents.
 */
typedef enum
{
    SCLI_TBL_ALIGN_LEFT = 0,
    SCLI_TBL_ALIGN_RIGHT,
    SCLI_TBL_ALIGN_CENTER,
} scli_tbl_align_t;

typedef struct
{
    size_t column_count;
    char **headers;
    size_t row_count;
    size_t row_capacity;
    char **cells;
    scli_tbl_align_t *alignments;
} scli_tbl_t;

/**
 * Allocate a table with @column_count columns.
 * Header strings are duplicated, so callers can pass temporary values.
 *
 * Every column starts left aligned; use scli_tbl_align() to change one.
 */
scli_tbl_t *scli_tbl_new(size_t column_count, char *headers[]);

/**
 * Set the alignment of a single column.
 *
 * Numeric columns usually read better right aligned. Returns false when
 * @table is NULL or @column is out of range.
 */
bool scli_tbl_align(scli_tbl_t *table, size_t column, scli_tbl_align_t alignment);

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
