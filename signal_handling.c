#include "minishell.h"

void handle_signals(int signo) {
    if (signo == SIGINT) {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}
