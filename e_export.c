#include "minishell.h"

void	execute_export(char **argv, t_data *data)
{
	int	i;

	if (!argv[1])
	{
		t_env *curr = data->env_list;
		while (curr)
		{
			if (curr->value)
				printf("declare -x %s=\"%s\"\n", curr->name, curr->value);
			else
				printf("declare -x %s\n", curr->name);
			curr = curr->next;
		}
		return;
	}
	i = 1;
	while (argv[i])
	{
		export_var(&argv[i], &(data->env_list));
		i++;
	}
}

void	export_var(char **argv, t_env **env_list)
{

	char *equal = ft_strchr(*argv, '=');
	char *name;
	char *value = NULL;
	t_env *curr;

	if (equal)
	{
		name = ft_strndup(*argv, equal - *argv);
		value = ft_strdup(equal + 1);
	}
	else
	{
		name = ft_strdup(*argv);
	}
	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->name, name, ft_strlen(name)) == 0)
		{
			if (value)
			{
				free(curr->value);
				curr->value = value;
			}
			free(name);
			return;
		}
		curr = curr->next;
	}
	t_env *new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->name = name;
	new_node->value = value;
	new_node->next = *env_list;
	*env_list = new_node;
}