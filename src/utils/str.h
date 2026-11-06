/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_STR_
#define _H_STR_

/* Evaluates its argument twice: do not pass an expression with side effects. */
#define str_safe(val) ((val) ? (val) : "")

#endif /* _H_STR_ */
