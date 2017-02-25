# Project 1 - Develop your own shell

Benoît Lafon, Sylvain Cassiau

CS551 OS Design & Implementation

Illinois Institute of Technology


## Repository content

* `doc/` contains the project report
* `src/` contains all the project's source code (see "How to build the shell" to compile it)
* `tools/` contains some programs we used during the development phase
* `tst/` contains test scripts (see "How to run tests")

## Summary

This project aims at building a custom shell for Minix 3.

It supports sequences of commands followed by `;` or `&` separators specifying if they are run in background or not. Commands can be grouped  using `(` and `)` and their standard input and output can be piped using `|` from one to another. Variables can also be defined and used.

Therefore, the following is a valid command line for our shell:

`msg="Hello World!" ; (ls -la | wc -l ; uname -a) & echo $msg`

The shell uses a profile file, has a customizable `$PATH` and `$HOME`, has an history of previously typed commands surviving shell exit, has an autocompletion based on the history that the user can scroll using [TAB] [UP ARROW] and [DOWN ARROW], supports killing running tasks with Ctrl-C (suggested after a toggleable 5 seconds timout), and can run script files passed in argument.


## How to build the shell

The project uses bison and lex to generate the parser. Make sure that these tools are installed on your Minix: `pkgin install lex bison`.

It is built with `clang` and `ld`. Make sure they are installed on your Minix: `pkgin install clang binutils`.

The source code is provided with a GNU makefile named `GNUmakefile` that you can run with the `gmake` command. The default `make` command in Minix 3 is BSD make, and is not totally compatible with GNU make syntax. You can install `gmake` with `pkgin install gmake`.

In order to create the binary `src/shell`, simply run `gmake` from the root folder of the repository.

| <span style="font-weight:normal">**Note:** `clang` generates an error if you try to build the project in a VirtualBox shared folder. If you need to do so, first copy `src/`'s contents to another directory.</span> |
| :------ |

## How to run the shell

You first need to build the shell following "How to build the shell" instructions.

You can then run `./src/shell` from Ash.

The shell program uses the file `/root/shell_profile` which is an initialization script that you can edit/create to customize the shell. Here is a sample content for this file:
```sh
PATH=/bin:/usr/bin     # folders used to look up for commands. use ':' as a separator.
HOME=/root             # the home directory for the shell.
ALARM=true             # toggle warning if a command is long to execute.
                       # you can call any valid command in this script.
echo Hello             # what about a welcome message?
```

The shell saves the command history to the file `/root/shell_history`. If the file does not exist, it will be created.

## How to run tests

You can run automated tests from the root folder of the directory with the command `gmake test`. You need to install Python 2.7 `pkgin install python27` first.

UI tests are performed manually.
