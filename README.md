# SparkCLI

[![Build](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/build.yml/badge.svg)](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/build.yml)
[![CodeQL](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/codeql.yml/badge.svg)](https://github.com/antoniovazquezblanco/sparkcli/actions/workflows/codeql.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE.md)

SparkCLI is a library to get the most out of console command line interfaces for programs written in C language.

## Features

<details>
<summary>Color Support</summary>

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
