/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_CONSOLE_
#define _H_CONSOLE_

#include <stdbool.h>

/**
 * Test whether our current file descriptor refers to a terminal.
 */
bool console_isatty();

/**
 * Test wether our current console is a dumb terminal.
 */
bool console_isdumb();

#endif /* _H_CONSOLE_ */