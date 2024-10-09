#include "minishell.h"

void check_and_update_shlvl(t_data *data, t_mini *mini_cmd)
{
	if (mini_cmd && mini_cmd->full_cmd && ft_strncmp(mini_cmd->full_cmd[0], "./minishell", ft_strlen("./minishell") + 1) == 0)
	 {
		data->mini_count++;
		update_shlvl(data, mini_cmd);
	}
}

//void	update_shlvl(t_data *data, t_mini *mini_cmd)
//{
//	char	*var_value;
//	int		i;
//	char	*new_value;

//	var_value = find_env_ref(data->env_list, "SHLVL");
//	if (!var_value)
//		return ;
//	i = ft_atoi(var_value);
//	//this needs to be changed, cause it should cound from export number too, not just go back to original mini number
//	if (ft_strncmp(mini_cmd->full_cmd[0], "./minishell", ft_strlen("./minishell") + 1) == 0)
//	{
//		i++;
//		new_value = ft_itoa(i);
//		if (!new_value)
//			return ;
//		update_env(&(data->env_list), "SHLVL", new_value);
//		free(new_value);
//	}
//}

void update_shlvl(t_data *data, t_mini *mini_cmd)
{
	char	*var_value;
	int		i;
	char	*new_value;

	var_value = find_env_ref(data->env_list, "SHLVL");
	if (!var_value)
		return ;
	i = ft_atoi(var_value);
	t_shlvl_node *new_node = malloc(sizeof(t_shlvl_node));
	if (!new_node)
		return ;
	new_node->shlvl_value = i;
	new_node->next = data->shlvl_history;
	data->shlvl_history = new_node;
	if (ft_strncmp(mini_cmd->full_cmd[0], "./minishell", ft_strlen("./minishell") + 1) == 0)
	{
		i++;
		new_value = ft_itoa(i);
		if (!new_value)
			return ;
		update_env(&(data->env_list), "SHLVL", new_value);
		free(new_value);
	}
}

void free_shlvl_history(t_data *data)
{
	t_shlvl_node *current = data->shlvl_history;
	while (current) {
		t_shlvl_node *next = current->next;
		free(current);
		current = next;
	}
	data->shlvl_history = NULL;
}
