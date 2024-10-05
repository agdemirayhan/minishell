#include "minishell.h"

void	check_shlvl(t_data *data, t_mini *mini_cmd)
{
	char	*var_value;
	int		i;
	char	*new_value;

	var_value = find_env_ref(data->env_list, "SHLVL");
	if (!var_value)
		return ;
	i = ft_atoi(var_value);
	while (i > 1)
	{
		if (ft_strncmp(mini_cmd->full_cmd[0], "exit", ft_strlen("exit") + 1) == 0)
		{
			i--;
			new_value = ft_itoa(i);
			if (!new_value)
				return ;
			update_env(&(data->env_list), "SHLVL", new_value);
			free(new_value);
			break ;
		}
		else
		{
			break;
		}
	}
}

