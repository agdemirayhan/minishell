#include "minishell.h"

/**
 * @brief Removes env variable
 * @param name name of the variable to remove
 * @param env_list list of environment variables.
 */
void	unset_var(char *name, t_env **env_list)
{
	t_env *curr;
	t_env *prev;

	curr = *env_list;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->name, name, ft_strlen(name) + 1) == 0)
		{
			if (prev == NULL)
			{
				*env_list = curr->next;
			}
			else
			{
				prev->next = curr->next;
			}
			free(curr->name);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

/**
 * @brief executes unset_var to remove the varible
 * @param argv argv[1] contains the variable name to remove
 * @param data structure containing the environment variable list
 */
void	execute_unset(char **argv, t_data *data)
{
	int	i;

	if (!argv[1])
	{
		return ;
	}
	i = 1;
	while (argv[i])
	{
		unset_var(argv[i], &(data->env_list));
		i++;
	}
}
