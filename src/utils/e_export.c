#include "../minishell.h"


/**
 * @brief Validates an environment variable name.
 * @param name The name of the variable to validate.
 * @return 1 if valid, 0 if invalid.
 */
int is_valid_var_name(const char *name)
{
	if (!name || !*name)
		return 0;

	if (!ft_isalpha(*name))
		return 0;

	for (int i = 1; name[i] != '\0'; i++)
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return 0;
	}
	for (int i = 0; name[i] != '\0'; i++)
	{
		if (ft_isalpha(name[i]))
			return 1;
	}

	return 0;
}


/**
 * @brief Executes the export command
 * @param argv The command line arguments passed to the export command
 * @param data Contains the environment variable list
 */
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
		export_var(&argv[i], &(data->env_list), data);
		i++;
	}
}

/**
 * @brief Updates or adds an environment variable.
 * @param argv Contains the variable name and optionally value.
 * @param env_list Points to the list of environment variables.
 */
void	export_var(char **argv, t_env **env_list, t_data *data)
{

	char *equal = ft_strchr(*argv, '=');
	char *name;
	char *value = NULL;
	t_env *curr;
	t_env *new_node;

	if (equal)
	{
		name = ft_strndup(*argv, equal - *argv);
		value = ft_strdup(equal + 1);
	}
	else
	{
		name = ft_strdup(*argv);
	}
	if (!is_valid_var_name(name))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(name);
		free(value);
		data->prev_exit_stat = 1;
		return;
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
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->name = name;
	new_node->value = value;
	new_node->next = *env_list;
	*env_list = new_node;
}