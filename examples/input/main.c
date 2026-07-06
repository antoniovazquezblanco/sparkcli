/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <sparkcli.h>
#include <stdio.h>
#include <stdlib.h>

const char *BANNER =
    "  ___                _    ___ _    ___ \n"
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n"
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n"
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n"
    "     |_|                               \n";

int main(void)
{
    printf("%s", BANNER);
    printf("Input helpers example for SparkCLI\n\n");

    // Read a single keystroke, no newline required.
    printf("Press any key to continue...");
    fflush(stdout);
    int key = scli_inp_key();
    printf("\nYou pressed: '%c' (0x%02x)\n\n", (key >= 32 && key < 127) ? key : '?', (unsigned char)key);

    // Read an arbitrary length line.
    char *name = scli_inp_line("What is your name? ");
    if (name)
    {
        printf("Hello, %s!\n", name);
        free(name);
    }

    return 0;
}
