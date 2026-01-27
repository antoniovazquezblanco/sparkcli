/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "log.h"

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

/* Log levels */
typedef enum
{
    SCLI_LOGLEVEL_DEBUG = 0,
    SCLI_LOGLEVEL_INFO,
    SCLI_LOGLEVEL_WARN,
    SCLI_LOGLEVEL_ERROR,
    SCLI_LOGLEVEL_NONE
} scli_loglevel_t;

char *_loglevel_strings[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "NONE"
};

/* Track last log timestamp to avoid duplicate timestamps */
static time_t g_last_log_time = 0;

/* Get current timestamp string */
static void _timestamp(char *buffer, size_t buffer_size, time_t now)
{
    struct tm *tm_info = localtime(&now);
    strftime(buffer, buffer_size, "%H:%M:%S", tm_info);
}

/* Core logging function */
static void _log_print(scli_loglevel_t level, const char *format, va_list args)
{
    time_t now = time(NULL);
    bool show_timestamp = (now != g_last_log_time);
    g_last_log_time = now;

    /* Print timestamp if needed */
    if (show_timestamp)
    {
        char timestamp[9];
        _timestamp(timestamp, sizeof(timestamp), now);
        printf("[%s] ", timestamp);
    }
    else
    {
        printf("           ");
    }

    /* Print level */
    printf("%-5s ", _loglevel_strings[level]);

    /* Print message */
    vprintf(format, args);
    printf("\n");
    fflush(stdout);
}

void scli_log_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_DEBUG, format, args);
    va_end(args);
}
void scli_log_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_INFO, format, args);
    va_end(args);
}
void scli_log_warn(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_WARN, format, args);
    va_end(args);
}
void scli_log_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_ERROR, format, args);
    va_end(args);
}
