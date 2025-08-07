# Mini Shell in C

A simple Unix-like shell implemented in C.

## Features

* Custom prompt showing current working directory
* Execute system commands using `fork()` and `execvp()`
* Background process execution using `&`
* Built-in commands:

  * `cd` to change directories
  * `exit` to quit the shell
* Handles zombie processes using `SIGCHLD` and `waitpid`

## How to Compile

```
gcc -o mysh main.c
```

## How to Run

```
./mysh
```

## Example Usage

```
/home/user> ls
/home/user> cd /tmp
/tmp> sleep 10 &
/tmp> exit
```
