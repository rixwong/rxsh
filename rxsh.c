#include "rxsh.h"

void init();

int main() {

    init();
    int bk_std_in = dup(0);
    int bk_std_out = dup(1);
    while (1) {

        prompt();

        char *origin_line = NULL;
        char **args = {NULL};

        origin_line = read_line();
        args = parse_line(origin_line);

        run(args);

        free(args);
        free(origin_line);

        dup2(bk_std_in, 0);
        dup2(bk_std_out, 1);        
    }
    return 0;
}

int run(char **args) {
    if (args[0] == NULL) {          // empty
        return 0;
    }
    if (try_buildin(args) == 0) {
        return 0;
    }

    int tfrom = 0, tto = 0;         // tfrom ~ tto

    for (tto = 0; args[tto] != NULL; tto++) {
        if (strcmp(args[tto], "|") != 0) 
            continue;

        // here we meet a "|"
        if (args[tto + 1] == NULL) {
            fprintf(stderr, "syntax error, need filename after \'|\'\n");
            return -1;
        }
        if (strcmp(args[tto + 1], "|") == 0) {              // the case is like "ls | | "
            fprintf(stderr, "syntax error, need filename after \'|\'\n");
            return -1;
        }
        //
        args[tto] = NULL;
        
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed!\n");
            return -1;
        } else if (pid == 0) {      // child progress
            close(fd[0]);
            dup2(fd[1], 1);
// printf("***in***");
            run_without_pipe(args + tfrom);
            exit(-1);        // if execvp (in run_without_pipe()) successfully, code won't run to here

        } else {                    // father progress
            int status;
            wait(NULL);
            close(fd[1]);
            dup2(fd[0], 0);
        }

        tfrom = tto + 1;            // 
        // else continue
    }

    // then we have a rest argument list to run finally
    // TODO:
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed!\n");
        return -1;
    } else if (pid == 0) {      // child progress
// printf("***out***");
        run_without_pipe(args + tfrom);
        exit(-1);        // if execvp (in run_without_pipe()) successfully, code won't run to here
    } else {
        int status;
        wait(NULL);
    }

    // close all things need to be closed ( if need ?)
}

void run_without_pipe(char **args) {

    // printf("***** Entering run_without_pipe()\n");

    int outflag = 0;                // outflag: 0: no >/>>, 1: >, 2: >>
    int inflag = 0;                 // inflag: 0: no <,   1: <
    char *infile = NULL, *outfile = NULL;
    int pos = 0;
    for (pos = 0; args[pos] != NULL; pos++) {
        if (strcmp(args[pos], "<") == 0) {
            if (args[pos + 1] == NULL) {
                fprintf(stderr, "syntax error, need filename after redirection operator\n");
                exit(1);
            }
            inflag = 1;
            infile = args[pos + 1];
            args[pos] = NULL;       // !
        } else if (strcmp(args[pos], ">") == 0) {
            if (args[pos + 1] == NULL) {
                fprintf(stderr, "syntax error, need filename after redirection operator\n");
                exit(1);
            }
            outflag = 1;
            outfile = args[pos + 1];
            args[pos] = NULL;
        } else if (strcmp(args[pos], ">>") == 0) {
            if (args[pos + 1] == NULL) {
                fprintf(stderr, "syntax error, need filename after redirection operator\n");
                exit(1);
            }
            outflag = 2;
            outfile = args[pos + 1];
            args[pos] = NULL;
        }
    }

    if (inflag == 1) {
        int infd = open(infile, O_RDONLY);
        dup2(infd, 0);
    }
    if (outflag == 1) {
        int outfd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(outfd, 1);
    } else if (outflag == 2) {
        int outfd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
        dup2(outfd, 1);
    }

    if (execvp(args[0], args) == -1) {
        fprintf(stderr, "error, command not found\n");
        exit(1);
    }
}

/**
 * if is a buildin_command, do it, return 0
 */
int try_buildin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        buildin_cd(args);
        return 0;
    } else if (strcmp(args[0], "help") == 0) {
        buildin_help(args);
        return 0;
    } else if (strcmp(args[0], "exit") == 0) {
        buildin_exit(args);
        return 0;
    }
    return -1;
}

void init() {
    printf("Welcome to rxsh\n");
    printf("Enter \"help\" to seek for help\n");
}

