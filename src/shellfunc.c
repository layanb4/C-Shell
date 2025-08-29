
#include "../include/shellfunc.h"
#include "../include/msgs.h"
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

char *inputhist[ARR_LEN];
int historynum = 0;
int totalcomnum = 0;
void cd(char *items[]) {
  const char *direrror = FORMAT_MSG("cd", CHDIR_ERROR_MSG);
  char *directoryinfo;
  static char previous[PATH_LEN] = "";
  char cur[PATH_LEN];
  char *homedir = getpwuid(getuid())->pw_dir;

  int numtokens = 0;
  while (items[numtokens] != NULL) {
    numtokens += 1;
  }

  if ((strcmp(items[0], "cd") == 0)) {

    if (numtokens > 2) {
      const char *toomany = FORMAT_MSG("cd", TMA_MSG);
      write(STDERR_FILENO, toomany, strlen(toomany));
      return;
    }

    getcwd(cur, sizeof(cur));

    if (items[1] == NULL) {
      directoryinfo = homedir;
    }

    else if ((strcmp(items[1], "~") == 0)) {
      directoryinfo = homedir;
    }

    else if ((items[1][0] == '~') && (items[1][1] == '/')) {
      char *temp = items[1] + 1;
      strcpy(cur, homedir);
      strcat(cur, items[1] + 1);
      directoryinfo = cur;
    }

    else if (strcmp(items[1], "-") == 0) {
      directoryinfo = previous;
    }

    else {
      directoryinfo = items[1];
    }

    if ((chdir(directoryinfo)) == -1) {
      write(STDERR_FILENO, direrror, strlen(direrror));
    } else {
      strcpy(previous, cur);
    }
  }
}

void helping(char *items[]) {

  int numtokens = 0;
  while (items[numtokens] != NULL) {
    numtokens += 1;
  }
  const char *cdm = FORMAT_MSG("cd", CD_HELP_MSG);
  const char *elsem = FORMAT_MSG("", EXTERN_HELP_MSG);
  const char *pwdm = FORMAT_MSG("pwd", PWD_HELP_MSG);
  const char *exitm = FORMAT_MSG("exit", EXIT_HELP_MSG);
  const char *helpmsg = FORMAT_MSG("help", HELP_HELP_MSG);
  const char *histmsg = FORMAT_MSG("history", HISTORY_HELP_MSG);
  if (numtokens == 1 && strcmp(items[0], "help") == 0) {
    write(STDOUT_FILENO, exitm, strlen(exitm));
    write(STDOUT_FILENO, pwdm, strlen(pwdm));
    write(STDOUT_FILENO, cdm, strlen(pwdm));
    write(STDOUT_FILENO, helpmsg, strlen(helpmsg));
    write(STDOUT_FILENO, histmsg, strlen(histmsg));
  }

  else if (numtokens > 1) {
    if (items[1] != NULL && items[2] == NULL) {
      if (strcmp(items[1], "exit") == 0) {
        write(STDERR_FILENO, exitm, strlen(exitm));
      }

      else if (strcmp(items[1], "pwd") == 0) {
        write(STDOUT_FILENO, pwdm, strlen(pwdm));
      } else if (strcmp(items[1], "cd") == 0) {
        write(STDOUT_FILENO, cdm, strlen(cdm));
      } else if (strcmp(items[1], "help") == 0) {
        write(STDOUT_FILENO, helpmsg, strlen(helpmsg));
      } else {
        write(STDOUT_FILENO, items[1], strlen(items[1]));
        write(STDOUT_FILENO, ": ", strlen(": "));
        write(STDOUT_FILENO, "external command or application",
              strlen("external command or application"));
        write(STDOUT_FILENO, "\n", strlen("\n"));
      }
    } else if (numtokens >= 3) {
      const char *toomany = FORMAT_MSG("help", TMA_MSG);
      write(STDERR_FILENO, toomany, strlen(toomany));
    }
  }
}

void exiting(char *items[]) {

  int numtokens = 0;
  while (items[numtokens] != NULL) {
    numtokens += 1;
  }

  if ((strcmp(items[0], "exit") == 0) && (items[1] == NULL)) {
    exit(0);
    return;
  }

  else if (numtokens > 1) {
    const char *exitmsg = FORMAT_MSG("exit", TMA_MSG);
    write(STDERR_FILENO, exitmsg, strlen(exitmsg));
    return;
  }
}

void pwd(char *items[]) {
  if (strcmp(items[0], "pwd") == 0 && (items[1] == NULL)) {
    char buffer[1024];
    char *getworkdir = getcwd(buffer, 1024);
    if ((getworkdir != NULL)) {
      write(STDOUT_FILENO, getworkdir, strlen(getworkdir));
      write(STDOUT_FILENO, "\n", strlen("\n"));
    } else {
      const char *getpwderror = FORMAT_MSG("pwd", GETCWD_ERROR_MSG);
      write(STDERR_FILENO, getpwderror, strlen(getpwderror));
    }
  }

  else if ((strcmp(items[0], "pwd") == 0) && (items[1] != NULL)) {
    const char *pwderror = FORMAT_MSG("pwd", TMA_MSG);
    write(STDERR_FILENO, pwderror, strlen(pwderror));
    return;
  }
}
// referenced dr.hafeeda's website
int tokenizer(char *input, char *items[]) {
  int elemcount = 0;

  char *inputstring = input;
  char *delim = " \t\n\r";
  char *token = NULL;
  char *saveptr = NULL;

  while ((token = strtok_r(inputstring, delim, &saveptr))) {
    items[elemcount] = token;
    elemcount++;

    inputstring = NULL;
  }

  items[elemcount] = NULL;
  return elemcount;
}

char **tokenizertwo(char *input, char *items[]) {
  int elemcount = 0;
  static char temp[1024];
  strcpy(temp, input);
  // char *inputstring = strdup(input);
  char *delim = " \t\n\r";
  char *token = NULL;
  char *saveptr = NULL;
  char *inputstring = temp;
  while ((token = strtok_r(inputstring, delim, &saveptr))) {
    items[elemcount] = token;
    elemcount++;

    inputstring = NULL;
  }

  items[elemcount] = NULL;
  return items;
}

void history() {

  for (int i = historynum - 1; i >= 0; i--) {

    char number[100];
    sprintf(number, "%d\t", ((totalcomnum - historynum) + i));
    write(STDOUT_FILENO, number, strlen(number));
    write(STDOUT_FILENO, inputhist[i], strlen(inputhist[i]));
  }
}

void removeoldestone() {
  if (historynum >= ARR_LEN) {
    free(inputhist[0]);
    for (int i = 1; i < historynum; i++) {
      inputhist[i - 1] = inputhist[i];
    }

    historynum--;
  }
}

void addhistory(char *input) {
  if ((historynum >= ARR_LEN)) {
    removeoldestone();
  }
  inputhist[historynum] = (char *)malloc(strlen(input) + 12);
  snprintf(inputhist[historynum], strlen(input) + 12, "%s\n", input);
  historynum++;
  totalcomnum++;
}
// referenced from dr.hafeedas website
int creader(char *input, int *checker, int inputlen, char *items[],
            bool *groundstatus) {
  *groundstatus = false;
  inputlen = read(STDIN_FILENO, input, PATH_LEN - 1);
  if (inputlen < 0) {
    if (errno == EINTR) {
      return -1;
    } else {
      const char *msg = FORMAT_MSG("shell", READ_ERROR_MSG);
      write(STDERR_FILENO, msg, strlen(msg));
    }
  }

  else if (inputlen == 0) {
    return 0;
  }

  else if (inputlen > 0) {

    input[inputlen] = '\0';
    if (input[inputlen - 1] == '\n') {
      input[inputlen - 1] = '\0';
    }

    if (input[0] != '!') {
      addhistory(input);
    }

    int numofitems = tokenizer(input, items);
    if (items[0] == NULL) {
      *checker = -1;
      return -1;
    }
    if (numofitems > 0 && strcmp(items[numofitems - 1], "&") == 0) {
      *groundstatus = true;
      items[numofitems - 1] = NULL;
    }
  }
  return 0;
}

void doit(char *input, bool *groundstatus, char *items[]) {
  if (strcmp(items[0], "pwd") == 0) {
    pwd(items);
  }

  else if (strcmp(items[0], "cd") == 0) {
    cd(items);
  } else if (strcmp(items[0], "help") == 0) {
    helping(items);
  }

  else if (strcmp(items[0], "exit") == 0) {
    exiting(items);
  }

  else if (strcmp(items[0], "history") == 0) {
    history();
    // return;
  }

  else {
    pid_t pid = fork();
    while ((waitpid(-1, NULL, WNOHANG)) > 0)
      ;
    if (pid > 0) {
      int wstatus = 0;
      if (!(*groundstatus)) {
        if (waitpid(pid, &wstatus, 0) == -1) {
          const char *waitmsg = FORMAT_MSG("shell", WAIT_ERROR_MSG);
          write(STDERR_FILENO, waitmsg, strlen(waitmsg));
        }
      }
      if (WIFSIGNALED(wstatus)) {
        printf("terminated by signal %d\n", WTERMSIG(wstatus));
      }

    } else if (pid == -1) {
      const char *forkmsg = FORMAT_MSG("shell", FORK_ERROR_MSG);
      write(STDERR_FILENO, forkmsg, strlen(forkmsg));
    } else if (execvp(items[0], items) == -1) {
      const char *execmsg = FORMAT_MSG("shell", EXEC_ERROR_MSG);
      write(STDERR_FILENO, execmsg, strlen(execmsg));
      exit(EXIT_FAILURE);
    }
  }
}
// reference from the ta
void trimming(char *input) {
  int length = strlen(input);
  while (length > 0 && isspace((unsigned char)input[length - 1])) {
    input[length - 1] = '\0';
    length--;
  }
}
void historychecker(char *input) {
  const char *invhist = FORMAT_MSG("history", HISTORY_INVALID_MSG);
  if (historynum == 0) {
    const char *nohistyet = FORMAT_MSG("history", HISTORY_NO_LAST_MSG);
    write(STDERR_FILENO, nohistyet, strlen(invhist));
    return;
  }

  else if (strcmp(input, "!!") == 0) {
    int lastcommandindex = (historynum - 1) % ARR_LEN;
    write(STDOUT_FILENO, inputhist[lastcommandindex],
          strlen(inputhist[lastcommandindex]));
    strcpy(input, inputhist[lastcommandindex]);
    trimming(input);
    addhistory(input);
  }

  else {
    // input++;
    char *ifnum = input + 1;

    //    printf("this is the number: %s\n", input);
    for (int i = 0; i < strlen(ifnum); i++) {
      if ((!isdigit(ifnum[i]))) {
        write(STDERR_FILENO, invhist, strlen(invhist));
        return;
      }
    }

    // int nums = atoi(ifnum);
    int nums = atoi(ifnum);
    if (nums < 0 || nums > historynum || nums < (historynum - 10)) {
      write(STDERR_FILENO, invhist, strlen(invhist));
      return;
    }

    int cmdnum = (nums) % 10;
    write(STDOUT_FILENO, inputhist[cmdnum], strlen(inputhist[cmdnum]));
    strcpy(input, inputhist[cmdnum]);
    trimming(input);
    addhistory(input);
  }
}
