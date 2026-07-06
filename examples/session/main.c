/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <sparkcli.h>
#include <stdio.h>

const char *BANNER =
    "  ___                _    ___ _    ___ \n" \
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n" \
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n" \
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n" \
    "     |_|                               \n";

static const char *yesno(bool value)
{
    return value ? "yes" : "no";
}

int main(void)
{
    printf("%s", BANNER);
    printf("Session capabilities example!\n\n");

    printf("SparkCLI can inspect the current session to adapt its output.\n");
    printf("Try piping this program to a file or another command to see the\n");
    printf("reported capabilities change.\n\n");

    // Query capabilities one by one...
    printf("Interactive: %s\n", yesno(scli_ses_interactive()));
    printf("Colors:      %s\n", yesno(scli_ses_color()));
    printf("Truecolor:   %s\n", yesno(scli_ses_truecolor()));

    return 0;
}
