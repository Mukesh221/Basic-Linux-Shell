# Basic-Shell
### Shell implemented in C language

Files:
1) `shell.c`  - main file with full shell code with signal handling, etc.
2) `util.h`   - supporting header file containing declaration of certain functions and variables
3) `README`   - Documentation

### Features
==============

* Basic commands: `pwd`, `clear`,`ls` and `cd`

* `ls` command shows all filed in the current directory (hidden also).

* Exit shell using `exit` commmand or `Ctrl + c` SIGINT signal

* `history n` command to print last `n` commands or all of the previous commands if no second argument.

*  `mkdir` command make a folder.

*  `rmdir` command delete a empty folder.
