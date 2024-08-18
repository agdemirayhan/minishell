#include "minishell.h"

void init_shell(void) {
    signal(SIGINT, handle_signals);
    signal(SIGQUIT, SIG_IGN);  // Ignore quit signal
}

int main(void) {
    char *input;

    init_shell();  // Call it here
    while (1) {
        input = readline("minishell> ");
        if (!input)
            break;
        add_history(input);
        parse_command(input);
        free(input);
    }
    return 0;
}
