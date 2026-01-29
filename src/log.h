/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_LOG_
#define _H_LOG_

/* Logging functions */
void scli_log_debug(const char *format, ...);
void scli_log_info(const char *format, ...);
void scli_log_warn(const char *format, ...);
void scli_log_error(const char *format, ...);

#endif /* _H_LOG_ */
