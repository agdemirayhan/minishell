#include "minishell.h"

void	parse_command(char *input)
{
	char	*args[1024];
	int		i;

	i = 0;
	args[i] = strtok(input, " ");
	while (args[i] != NULL)
	{
		i++;
		args[i] = strtok(NULL, " ");
	}
	if (args[0] == NULL)
		return ;
	if (is_builtin(args[0]))
	{
		execute_builtin(args);
	}
	else
	{
		execute_command(args);
	}
}
