/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_LOG_
#define _H_LOG_

#if defined(__MINGW32__)
#define SCLI_PRINTF(fmt_index, args_index) __attribute__((format(gnu_printf, fmt_index, args_index)))
#elif defined(__GNUC__) || defined(__clang__)
#define SCLI_PRINTF(fmt_index, args_index) __attribute__((format(printf, fmt_index, args_index)))
#else
#define SCLI_PRINTF(fmt_index, args_index)
#endif

/* Logging functions */
void scli_log_debug(const char *format, ...) SCLI_PRINTF(1, 2);
void scli_log_info(const char *format, ...) SCLI_PRINTF(1, 2);
void scli_log_warn(const char *format, ...) SCLI_PRINTF(1, 2);
void scli_log_error(const char *format, ...) SCLI_PRINTF(1, 2);

#endif /* _H_LOG_ */
