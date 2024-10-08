#include "minishell.h"

int		g_status;

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\nminishell> ", 12);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}