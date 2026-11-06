/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _WIN32
/* Make nanosleep() visible regardless of the c_std in effect. */
#define _POSIX_C_SOURCE 200809L
#endif

#include <sparkcli.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

const char *BANNER =
    "  ___                _    ___ _    ___ \n" \
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n" \
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n" \
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n" \
    "     |_|                               \n";

/* Cross-platform sleep function */
static void sleep_ms(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    struct timespec delay = {.tv_sec = milliseconds / 1000, .tv_nsec = (milliseconds % 1000) * 1000000L};
    nanosleep(&delay, NULL);
#endif
}

int main(void)
{
    printf("%s", BANNER);
    printf("A logging functionality example!\n\n");

    /* Log at different levels */
    scli_log_debug("This is a debug message");
    scli_log_info("This is an info message");
    scli_log_warn("This is a warning message");
    scli_log_error("This is an error message");
    sleep_ms(2000);
    
    /* Demonstrate timestamp behavior with rapid messages */
    scli_log_info("First message");
    scli_log_info("Second message (same second)");
    scli_log_info("Third message (same second)");
    scli_log_info("Error message (same second)");
    sleep_ms(2000);
    scli_log_info("Message after 2 seconds");
    scli_log_info("Another warning after delay (same second)");
    sleep_ms(2000);
    scli_log_info("Final message");
    scli_log_info("Another final message (same second)");
    
    return 0;
}