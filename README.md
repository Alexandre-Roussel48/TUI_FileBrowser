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

## Install

   ```bash
   bash -c "$(curl -fsSL https://raw.githubusercontent.com/Alexandre-Roussel48/TUI_FileBrowser/main/install.sh)"
   ```

## Usage

0. **Execution**: Run the command or click on the logo in the applications center.

   ```bash
   nav
   ```

1. **Navigation**:

   - Use `TAB` to switch between the directory and file windows.
   - Press `ENTER` to 'cd' into a directory if the cursor is on a directory or 'open' a file if the cursor is on a file.
   - Use `KEY_UP` and `KEY_DOWN` to navigate through elements in the current window.
   - Press `KEY_RIGHT` to enter a directory.
   - Press `KEY_LEFT` to go back from a directory.

## Example

1. Start the program by executing the command set previously or by clicking the logo.

   ```bash
   nav
   ```

2. Navigate through the filesystem using the specified keys.

3. Enjoy exploring your Linux filesystem with a simple and intuitive interface!

## Notes

- The program is designed for Linux-type filesystems that runs with **bashrc**.
- Ensure that the ncurses library is installed on your system before compiling and running the program.

Feel free to explore, modify, and contribute to enhance the functionality of this filesystem browser. Happy browsing!