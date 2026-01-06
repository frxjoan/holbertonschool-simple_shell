# Simple Shell (hsh)

## Description
`hsh` is a simple UNIX command-line interpreter written in C. It replicates basic behavior of `/bin/sh`, allowing users to execute commands in both interactive and non-interactive modes.

This project is part of the Holberton School low-level programming curriculum and follows strict coding and style guidelines (Betty style).

---

## Features
- Interactive and non-interactive modes
- Executes commands using absolute paths
- Handles command errors like `/bin/sh`
- Proper memory management (no leaks)
- Uses system calls where appropriate

---

## Usage

### Interactive Mode
```bash
$ ./hsh
($) /bin/ls
file1 file2
($) exit
