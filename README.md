# C-Shell💻

This project implements a custom command-line shell in C, capable of executing both foreground and background processes, handling internal commands (cd, pwd, exit, help, and history), and maintaining a history of the last 10 commands. The shell features a prompt that displays the current working directory, supports re-running commands from history using !! and !n syntax, and gracefully handles the SIGINT signal (e.g., Ctrl+C) by displaying help instead of terminating. Built using CMake with a structured directory layout, the shell leverages key Unix system calls such as fork(), exec(), waitpid(), and chdir() to provide core shell functionalities, while ensuring proper input/output handling, background process cleanup, and robust error reporting via predefined macros.

## Prerequisites

* gcc installed
* linux machine

### Executing the program

* Enter `make` into your program terminal. Make sure you are in the correct directory!
* Enter `./shell` to run the executable

## 🛠️ Supported Commands

### 🔹 Internal Commands (Built-in)

- **`exit`**  
  Exits the shell.  
  - If any arguments are provided:  
    ```
    exit: too many arguments
    ```

- **`pwd`**  
  Prints the current working directory.  
  - If any arguments are provided:  
    ```
    pwd: too many arguments
    ```

- **`cd [path]`**  
  Changes the current working directory.  
  - `cd` with no argument → changes to the home directory  
  - `cd ~` → changes to the home directory  
  - `cd ~/folder` → changes to a folder under home directory  
  - `cd -` → switches to the previous directory  
  - If invalid:  
    ```
    cd: too many arguments
    cd: unable to change directory
    ```

- **`help [command]`**  
  Displays help information for internal commands.  
  - No arguments → lists all internal commands and descriptions  
  - One valid internal command → shows help for that command  
  - One invalid command → prints:  
    ```
    <command>: external command or application
    ```
  - More than one argument:  
    ```
    help: too many arguments
    ```

- **`history`**  
  Displays the last 10 commands entered (most recent first), including internal and external commands.

- **`!!`**  
  Re-runs the most recent command.  
  - If no previous commands:  
    ```
    history: no command entered
    ```

- **`!n`**  
  Re-runs command number `n` from the history list.  
  - If `n` is invalid or not within the last 10:  
    ```
    history: command invalid
    ```

---

### 🔹 External Commands

Any valid system command (e.g., `ls`, `cat`, `echo`, etc.) is executed in a separate child process.

- **Foreground Execution:**  
ls -la

- **Background Execution (with `&`):**  
sleep 10 &



