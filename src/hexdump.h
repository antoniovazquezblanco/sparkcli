/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Kevin Leon
 */
#ifndef _H_HEXDUMP_
#define _H_HEXDUMP_

#include <stdint.h>
#include <stddef.h>

/**
 * Print a formatted hex dump (hex + ASCII) with a custom line width.
 *
 * @param buffer       Pointer to the input data buffer.
 * @param buffer_len   Length of the buffer in bytes.
 * @param chunk_len    Number of bytes displayed per line.
 */
void scli_hexdump(const uint8_t *buffer, size_t buffer_len, size_t chunck_len);

/**
 * Print a formatted hex dump (hex + ASCII) with a custom line width.
 *
 * @param buffer       Pointer to the input data buffer.
 * @param buffer_len   Length of the buffer in bytes.
 */
void scli_hexdump8(const uint8_t *buffer, size_t buffer_len);
void scli_hexdump16(const uint8_t *buffer, size_t buffer_len);
void scli_hexdump32(const uint8_t *buffer, size_t buffer_len);

/**
 * Print hexadecimal values only (no ASCII) with a custom line width.
 *
 * @param buffer       Pointer to the input data buffer.
 * @param buffer_len   Length of the buffer in bytes.
 * @param chunk_len    Number of bytes displayed per line.
 */
void scli_hex(const uint8_t *buffer, size_t buffer_len, size_t chunck_len);

/**
 * Print hexadecimal values only (no ASCII) with a custom line width.
 *
 * @param buffer       Pointer to the input data buffer.
 * @param buffer_len   Length of the buffer in bytes.
 */
void scli_hex8(const uint8_t *buffer, size_t buffer_len);
void scli_hex16(const uint8_t *buffer, size_t buffer_len);
void scli_hex32(const uint8_t *buffer, size_t buffer_len);
#endif //_H_HEXDUMP_