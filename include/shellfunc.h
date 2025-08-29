#ifndef SHELL_FUNC_H
#define SHELL_FUNC_H
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#define PATH_LEN 1024
#define ARR_LEN 10

void cd(char *items[]);

void helping(char *items[]);

void exiting(char *items[]);

int tokenizer(char *input, char *items[]);

char **tokenizertwo(char *input, char *items[]);

void history();

void removeoldestone();

void addhistory(char *input);

int creader(char *input, int *checker, int inputlen, char *items[],
            bool *groundstatus);

void handle_sigint(int signum);

void doit(char *input, bool *groundstatus, char *items[]);

void trimming(char *input);

void historychecker(char *input);

#endif
