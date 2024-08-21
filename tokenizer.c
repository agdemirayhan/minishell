#include "minishell.h"

t_ttype	check_type(char *value)
{
	if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(value, "<", 1) == 0 || ft_strncmp(value, ">", 1) == 0)
		return (REDIRECTION);
	else
		return (WORD);
}