#include "rxsh.h"

int buildin_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "expected argument(s) to \"cd\"\n");
    } else if (chdir(args[1]) != 0) {
        fprintf(stderr, "cd: No such file or directory\n");
    }
    return 0;
}
int buildin_help(char **args) {
    printf("----- rxsh -----\n");
    printf("The following are buildin commands:\n");
    printf("  cd\n");
    printf("  help\n");
    printf("  exit\n");
}
int buildin_exit(char **args){
    exit(0);
}