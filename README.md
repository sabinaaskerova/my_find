# find functionality implementation

**Description:**

`custom_find` is a C program that recursively traverses a directory structure and executes a specified command on each regular file encountered. It mimics the functionality of the `find` command in UNIX systems but is implemented using only system primitives for directory traversal and process management.

**Features:**

- Recursively traverses directories, distinguishing between regular files, directories, and other types of entries.
- Executes a specified command on each regular file encountered during traversal.
- Handles errors gracefully and provides informative error messages.
- Complies with constraints provided in the task description, including limitations on the use of library functions and dynamic memory allocation.

**Usage:**

```
./custom_find [directory] [command]
```

- `directory`: The directory to start the traversal from.
- `command`: The command to execute on each regular file encountered.

**Exit Codes:**

- `0`: Success.
- `1`: System call failure.
- `2`: Other errors encountered during execution.

**Compilation:**

```
cc -Wall -Wextra -Werror custom_find.c -o custom_find
```

**Run:**

```
./custom_find [directory] [command]
```

**Testing:**

To run the provided tests:

```
make test
```
