/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <sparkcli.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char *BANNER =
    "  ___                _    ___ _    ___ \n"
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n"
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n"
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n"
    "     |_|                               \n";

/* A list of fake components to show in the table... */
char *components[] = {
    "Core",
    "CLI",
    "Renderer",
    "Backend",
    "Database",
    "Network",
};

/* Here is a fake status code that will be shown in a table... */
typedef enum
{
    STATUS_READY,
    STATUS_CONFIG,
    STATUS_IDLE,
    STATUS_DEGRADED,
    STATUS_ERROR,
} status_t;

char *status_msgs[] = {
    "Ready",
    "Configuring",
    "Idle",
    "Degraded",
    "Error",
};

status_t status_rand(void)
{
    return (status_t)(rand() % (sizeof(status_msgs) / sizeof(*status_msgs)));
}

/* Also, in the table, there will be a progres percentage */
unsigned char percent_random()
{
    return (unsigned char)(rand() % 101);
}

int main(void)
{
    srand((unsigned)time(NULL));

    printf(BANNER);
    printf("Table helper example for SparkCLI\n\n");

    printf("Tables can have emtpy cells or rows and column with is automagically calculated.\n");

    // Create a table...
    char *headers[] = {"Component", "Status", "Progress"};
    scli_tbl_t *table = scli_tbl_new(3, headers);
    if (!table)
        // If table allocation fails, return...
        return 1;

    // Add contents to the table...
    int number_of_rows = sizeof(components) / sizeof(*components);
    for (size_t row_index = 0; row_index < number_of_rows; ++row_index)
    {
        status_t status = status_rand();
        unsigned char percent = percent_random();

        char progress_text[8];
        snprintf(progress_text, sizeof(progress_text), "%u%%", percent);

        char *row[] = {
            components[row_index],
            status_msgs[status],
            progress_text,
        };

        scli_tbl_add_row(table, row);
    }

    printf("Example output:\n");
    scli_tbl_render(table);

    scli_tbl_free(&table);

    return 0;
}
