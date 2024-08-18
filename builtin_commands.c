#include "minishell.h"

int is_builtin(char *command) {
    return (strcmp(command, "cd") == 0 || strcmp(command, "exit") == 0);
}

void execute_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) != 0) {
            perror("minishell");
        }
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
}
