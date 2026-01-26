# SparkCLI

SparkCLI is a library to get the most out of console command line interfaces for programs written in C language.

## Features

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
