/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_CONSOLE_
#define _H_CONSOLE_

#include <stdbool.h>

/**
 * Test whether the standard output refers to a terminal.
 */
bool console_stdout_isatty();

/**
 * Test whether the standard input refers to a terminal.
 */
bool console_stdin_isatty();

/**
 * Test wether our current console is a dumb terminal.
 */
bool console_isdumb();

/**
 * Does the current console support true colors?.
 */
bool console_hastruecolor();

/**
 * Try to switch the console output to UTF-8 encoding.
 */
bool console_set_utf8_output(void);

#endif /* _H_CONSOLE_ */
