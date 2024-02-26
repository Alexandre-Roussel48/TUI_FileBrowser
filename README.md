# Filesystem Browser with ncurses and lpanel

## Overview

This C program provides a simple and interactive way to browse through your filesystem on a Linux-type system. It utilizes the ncurses library for creating a text-based user interface with two windows: the left window displays directories, and the right window displays files. The program follows a linked tree structure and employs system calls to perform 'ls' on the selected directories.

## Features

- **Two Windows Layout**: Left window for directories, right window for files.
- **Navigation**: Easily navigate through directories and files.
- **Linked Tree Structure**: Uses a linked tree to represent the filesystem structure.
- **System Calls**: Utilizes system calls for 'ls' functionality on selected directories.

## Dependencies

- **ncurses**: For creating a terminal-based interface.
- **panel.h**: For creating panels allowing browsing.
- **string.h**: For the manipulation of strings.
- **stdlib.h**: For dynamic allocation of memory.
- **stdio.h**: For displaying.
- **sys/stat.h**: For performing sys calls for 'ls'.

## Usage

0. **Path setup**: Set the initial path for the program.

   ```
   main.c @ line 37
   ```
   ```C
   execute(NULL, "dir_name", "dir_absolute_path");
   ```

1. **Compilation**: Compile the program using a C compiler.

   ```bash
   cd TUI_FileBrowser
   gcc main.c -lpanel -lncurses -o main
   ```

2. **Setup**: Create alias for global use.

   ```bash
   cd ~
   nano .bashrc
   >> alias nav='. path/to/TUI_FileBrowser/navigation'
   source .bashrc
   ```

3. **Execution**: Run the command set previously.

   ```bash
   nav
   ```

4. **Navigation**:

   - Use `TAB` to switch between the directory and file windows.
   - Press `ENTER` to 'cd' into a directory if the cursor is on a directory or 'open' a file if the cursor is on a file.
   - Use `KEY_UP` and `KEY_DOWN` to navigate through elements in the current window.
   - Press `KEY_RIGHT` to enter a directory.
   - Press `KEY_LEFT` to go back from a directory.

## Example

1. Start the program by executing the command set previously.

   ```bash
   nav
   ```

2. Navigate through the filesystem using the specified keys.

3. Enjoy exploring your Linux filesystem with a simple and intuitive interface!

## Notes

- The program is designed for Linux-type filesystems.
- Ensure that the ncurses library is installed on your system before compiling and running the program.

Feel free to explore, modify, and contribute to enhance the functionality of this filesystem browser. Happy browsing!
