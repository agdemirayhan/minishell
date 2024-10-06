#include "minishell.h"

int		g_status;

void	handle_signals(int signo)
{
	if (signo == SIGINT)
	{
		g_status = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("\n", 0);
		rl_redisplay();
	}
	exit(g_status);
}