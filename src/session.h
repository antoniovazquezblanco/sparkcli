/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_SESSION_
#define _H_SESSION_

#include <stdbool.h>

/**
 * Test whether the current session is interactive.
 *
 * A session is considered interactive when both the standard input and the
 * standard output are attached to a non-dumb terminal, meaning a user is
 * expected to be present to read output and provide input.
 */
bool scli_ses_interactive(void);

/**
 * Test whether the current session supports colored output.
 */
bool scli_ses_color(void);

/**
 * Test whether the current session supports 24-bit truecolor output.
 */
bool scli_ses_truecolor(void);

#endif /* _H_SESSION_ */
