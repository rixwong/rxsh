#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_BUF_LENGTH 1024

int buildin_cd(char **args);
int buildin_help(char **args);
int buildin_exit(char **args);
char* read_line();
char** parse_line(char *);

int try_buildin(char **args);

int run(char **args);
void run_without_pipe(char **args);

void prompt();