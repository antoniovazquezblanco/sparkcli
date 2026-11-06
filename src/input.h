/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_INPUT_
#define _H_INPUT_

#include "api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Read a single keystroke without waiting for a newline and without echoing it.
 *
 * Returns the character read, or EOF on end-of-input or error.
 */
SCLI_API int scli_inp_key(void);

/**
 * Print @prompt (when not NULL) and read a whole line from standard input.
 *
 * The buffer grows as needed, so lines of arbitrary length are supported. The
 * trailing newline is removed. Returns a heap-allocated string that the caller
 * must free(), or NULL on end-of-input or error.
 */
SCLI_API char *scli_inp_line(const char *prompt);

#ifdef __cplusplus
}
#endif

#endif /* _H_INPUT_ */
