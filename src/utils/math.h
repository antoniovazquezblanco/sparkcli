/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_MATH_
#define _H_MATH_

#ifdef _WIN32
#include <stdlib.h>
#endif

#ifndef min
#define min(a, b) (a < b ? a : b)
#endif

#ifndef max
#define max(a, b) (a > b ? a : b)
#endif

#endif /* _H_MATH_ */
