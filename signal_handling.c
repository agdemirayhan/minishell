#include "minishell.h"

//void	set_sig(int sig)
//{
//	(void)sig;
//	g_sig = sig;
//}
void	handle_signals(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
//ctrl c needs to be implemented
//void	set_signal_fn(void)
//{
//	rl_catch_signals = 0;
//	signal(SIGQUIT, SIG_IGN);
//}