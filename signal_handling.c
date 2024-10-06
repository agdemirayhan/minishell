#include "minishell.h"

int		g_status;
void	handle_signals(int signo)
{
	if (signo == SIGINT)
	{
		g_status = 130;         // Set exit code to 130 for SIGINT
		printf("\n");           // Print newline after Ctrl+C
		rl_on_new_line();       // Move readline to a new line
		rl_replace_line("", 0); // Clear the current line in readline buffer
		rl_redisplay();         // Redisplay the prompt
	}
}
// ctrl c needs to be implemented
// void	set_signal_fn(void)
//{
//	rl_catch_signals = 0;
//	signal(SIGQUIT, SIG_IGN);
//}