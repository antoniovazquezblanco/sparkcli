# SparkCLI

[![Build](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/build.yml/badge.svg)](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/build.yml)
[![CodeQL](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/codeql.yml/badge.svg)](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/codeql.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE.md)

SparkCLI is a library to get the most out of console command line interfaces for programs written in C language.

## Features

<details>
<summary>Colors</summary>

SparkCLI provides truecolor support for both foreground and background text colors, enabling rich console output:

```c
#include <sparkcli.h>

int main(void) {
    // Set foreground colors
    scli_color_fg((scli_color_t){255, 0, 0});
    printf("This is red text\n");

    scli_color_fg((scli_color_t){0, 255, 0});
    printf("This is green text\n");

    scli_color_fg((scli_color_t){0, 0, 255});
    printf("This is blue text\n");

    // Set background colors
    scli_color_bg((scli_color_t){255, 255, 0});
    printf("Yellow background");

    // Reset colors
    scli_color_reset();
    printf("\n");

    return 0;
}
```

**Key Features:**

- **Truecolor Support**: 24-bit RGB color precision
- **Foreground & Background**: Independent control of text and background colors
- **Cross-Platform**: Automatically detects terminal color support
- **Color Reset**: Easy cleanup with `scli_color_reset()`

The color system uses RGB values (0-255) for precise color control and gracefully handles terminals that don't support color output.

</details>

<details>
<summary>Logging</summary>

SparkCLI provides a logging system with multiple log levels and automatic timestamp management:

```c
#include <sparkcli.h>

int main(void) {
    scli_log_debug("This is a debug message");
    scli_log_info("This is an info message");
    scli_log_warn("This is a warning message");
    scli_log_error("This is an error message");
    return 0;
}
```

**Sample Output:**

```
[14:23:45] DEBUG  This is a debug message
           INFO   This is an info message
           WARN   This is a warning message
           ERROR  This is an error message
```

The logging system automatically manages timestamps to avoid redundancy when multiple messages are logged within the same second.

</details>

<details>
<summary>Tables</summary>

SparkCLI now ships with a table helper that makes it easy to lay out tabular data with automatic column sizing:

```c
#include <sparkcli.h>

int main(void)
{
    char *headers[] = {"Component", "Status", "Progress"};
    scli_tbl_t *table = scli_tbl_new(3, headers);
    if (!table)
        return 1;

    char *row[] = {"Renderer", "Ready", "100%"};
    scli_tbl_add_row(table, row);

    scli_tbl_render(table);
    scli_tbl_free(&table);
    return 0;
}
```

Sample output:

```
+-----------+--------+----------+
| Component | Status | Progress |
+-----------+--------+----------+
| Renderer  | Ready  | 100%     |
+-----------+--------+----------+
```

**Highlights:**

- Automatically adjusts column widths from header or row content.
- Duplicates strings internally, so you can pass literals or temporary buffers without manual lifetime management.
- Includes helpers for sharing rows, backing arrays, and cleanup through `scli_tbl_free()`.

The example in `examples/table` demonstrates building a randomized status/progress table to show how the API works in practice.

</details>

<details>
<summary>Progress Bars</summary>

SparkCLI ships with a tiny progress helper for simple CLI tasks:

```c
#include <sparkcli.h>

int main(void)
{
    scli_prg_t *progress = scli_prg_new(60, "Rendering");
    if (!progress)
        return 1;

    for (size_t tick = 0; tick <= 60; ++tick)
    {
        scli_prg_update(progress, tick);
        wait_ms(25);
    }
    scli_prg_finish(progress);
    scli_prg_free(progress);
    return 0;
}
```

Sample output:

```
Rendering [###############---------------]  50%
```

</details>
