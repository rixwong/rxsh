#include "rxsh.h"

char* read_line() {
    char *line = NULL;
    ssize_t len = 0;
    int r = getline(&line, &len, stdin);
    if (r == -1) {
        if (feof(stdin)) {
            exit(1);                        // act like bash, when user enter Ctrl-D, exit the shell
        } else {
            perror("unknown error when readline");
            exit(1);
        }
    }

    if (r >= 1 && (line[r - 1] == '\n' || line[r - 1] == '\r')) line[r - 1] = '\0';
    if (r >= 2 && (line[r - 2] == '\n' || line[r - 2] == '\r')) line[r - 2] = '\0';

    return line;
}

char** parse_line(char *line) {
    char **args = malloc(MAX_BUF_LENGTH * sizeof(char *));
    args[0] = NULL;                 // important !
    int argc = 0;
    char *ptr = line;
    int capital = 1;
    for (ptr = line; *ptr != '\n' && *ptr != '\0'; ptr++) {
        if (!isspace(*ptr) && capital) {
            capital = 0;
            args[argc++] = ptr;
        } else if (isspace(*ptr)) {
            capital = 1;
            *ptr = '\0';
        }
    }
    *ptr = '\0';

    args[argc] = NULL;

    //printf("%s\n", line);

    return args;
}

void prompt() {
    printf("$ ");
}