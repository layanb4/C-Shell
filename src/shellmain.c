
#include "../include/msgs.h"
#include "../include/shellfunc.h"
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

// i acknowledge all the help i received from my prof, tas, and peers
// chatgpt for helping me debug the history line format for !! and !,
// the format for the cd ~ and -, and correctly calculating numbers for printing
// the outputs such as historychecker and history
// I also acknowledge the online resources i used such as
// geek for geeks for the strncpy, stackoverflow for git help, and dr.hafeeda's
// cmpt300 website for explanations and break down of reading and tokenization
// structure

static char *message = "\n";
void handle_sigint(int signum) {
  write(STDOUT_FILENO, message, strlen(message));
  const char *cdm = FORMAT_MSG("cd", CD_HELP_MSG);
  const char *pwdm = FORMAT_MSG("pwd", PWD_HELP_MSG);
  const char *exitm = FORMAT_MSG("exit", EXIT_HELP_MSG);
  const char *helpmsg = FORMAT_MSG("help", HELP_HELP_MSG);
  const char *historym = FORMAT_MSG("history", HISTORY_HELP_MSG);
  write(STDOUT_FILENO, exitm, strlen(exitm));
  write(STDOUT_FILENO, pwdm, strlen(pwdm));
  write(STDOUT_FILENO, cdm, strlen(pwdm));
  write(STDOUT_FILENO, helpmsg, strlen(helpmsg));
  write(STDOUT_FILENO, historym, strlen(historym));
  return;
}
int main() {
  size_t inputlen = 0;
  bool set = false;
  bool groundstatus = false;
  char *items[PATH_LEN];
  char buff[PATH_LEN + 1];
  char input[PATH_LEN];
  bool notinclude = false;
  int checker = 0;

  struct sigaction act;
  act.sa_handler = handle_sigint;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  int ret = sigaction(SIGINT, &act, NULL);
  if (ret == -1) {
    perror("Sigaction() failed");
    exit(EXIT_FAILURE);
  }

  while (true) {
    char *workdir = getcwd(buff, PATH_LEN + 1);
    if (workdir != NULL) {
      write(STDOUT_FILENO, workdir, strlen(workdir));
      write(STDOUT_FILENO, "$ ", strlen("$ "));
    }

    else {
      const char *getcwdmsg = FORMAT_MSG("shell", GETCWD_ERROR_MSG);
      write(STDERR_FILENO, getcwdmsg, strlen(getcwdmsg));
    }

    groundstatus = false;
    if (creader(input, &checker, inputlen, items, &groundstatus) == -1) {
      ;
      continue;
    }

    if (input[0] == '!') {
      historychecker(input);
      tokenizertwo(input, items);
    }

    doit(input, &groundstatus, items);
  }
}
