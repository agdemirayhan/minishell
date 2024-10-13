#include "minishell.h"

int	is_builtin(char *command)
{
	return (ft_strncmp(command, "echo", ft_strlen("echo")) == 0
		|| ft_strncmp(command, "cd", ft_strlen("cd")) == 0
		|| ft_strncmp(command, "pwd", ft_strlen("pwd")) == 0
		|| ft_strncmp(command, "export", ft_strlen("export")) == 0
		|| ft_strncmp(command, "unset", ft_strlen("unset")) == 0
		|| ft_strncmp(command, "env", ft_strlen("env")) == 0
		|| ft_strncmp(command, "exit", ft_strlen("exit")) == 0);
}

/*joins strings with spaces between them*/
char	*join_space(char **args)
{
	size_t	total_length;
	int		i;
	char	*result;

	total_length = 0;
	i = 0;
	while (args[i] != NULL)
	{
		total_length += ft_strlen(args[i]) + 1;
		i++;
	}
	result = malloc(total_length);
	if (result == NULL)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (args[i] != NULL)
	{
		ft_strlcat(result, args[i], total_length);
		if (args[i + 1] != NULL)
			ft_strlcat(result, " ", total_length);
		i++;
	}
	return (result);
}

/*joins two strings and frees the first one*/
char	*free_strjoin(char *str1, const char *str2)
{
	size_t	new_length;
	char	*result;

	new_length = ft_strlen(str1) + ft_strlen(str2) + 1;
	result = malloc(new_length);
	if (result == NULL)
	{
		free(str1);
		return (NULL);
	}
	ft_strlcpy(result, str1, new_length);
	ft_strlcat(result, str2, new_length);
	free(str1);
	return (result);
}

char	*get_dir(char **args)
{
	char	*home;
	char	*home1;

	if (!args[1] || ft_strlen(args[1]) == 0)
	{
		home = getenv("HOME");
		if (!home)
			return (NULL);
		return (ft_strdup(home));
	}
	if (ft_strncmp(args[1], "~", 1) == 0)
	{
		home1 = getenv("HOME");
		if (!home1)
			return (NULL);
		return (ft_strjoin(home1, args[1] + 1));
	}
	return (ft_strdup(args[1]));
}

static int	check_exit_num(char *arg, int *exit_code)
{
	int	i;
	int	num;

	i = 0;
	while (arg[i] == ' ' || arg[i] == '\t')
		i++;
	num = i;
	while (arg[num] != '\0')
	{
		if (arg[num] != '-' && arg[num] != '+'
			&& !ft_isdigit(arg[num]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			return (1);
		}
		num++;
	}
	*exit_code = ft_atoi(&arg[i]);
	if (*exit_code > 255)
		*exit_code = *exit_code % 256;
	if (*exit_code < 0)
		*exit_code = 256 + *exit_code;
	return (0);
}

void	execute_builtin(char **args, t_data *data)
{
	char	cwd[1024];
	int		i;
	char	*dir;
	int		newline;
	char	*str;

	// what happens when someone tries it with a directory which requires more than 1024 chars????? fails. using malloc for this?? eww. <--this is actually ok
	// also it needs to handle cd ~ so it can go home ewwwwww.
	if (ft_strncmp(args[0], "cd", ft_strlen("cd")) == 0)
	{
		// if (chdir(args[1]) != 0)
		//	perror("minishell");
		dir = get_dir(args);
		if (!dir)
			return ;
		update_old_pwd(&(data->env_list));
		if (chdir(dir) != 0)
		{
			perror("minishell");
		}
		else
			update_pwd(&(data->env_list));
		free(dir);
	}
	// this probably needs fixing too
	else if (ft_strncmp(args[0], "exit", ft_strlen("exit")) == 0)
	{
		int	exit_code = 0;

		printf("exit\n");
		if (args[1] != NULL)
		{
			if (check_exit_num(args[1], &exit_code))
			{
				data->prev_exit_stat = 1;
				if (data->shlvl_history)
				{
					int previous_shlvl = data->shlvl_history->shlvl_value;
					t_shlvl_node *temp = data->shlvl_history;
					data->shlvl_history = data->shlvl_history->next;
					free(temp);
					if (data->mini_count == 1)
					{
						clean_up(data);
						exit(data->prev_exit_stat);
					}
					else
					{
						data->mini_count--;
						if (previous_shlvl > 0)
						{
							char *new_value = ft_itoa(previous_shlvl);
							if (new_value)
							{
								update_env(&(data->env_list), "SHLVL", new_value);
								free(new_value);
							}
						}
						return;
					}
				}
				else
				{
					if (data->mini_count == 1)
					{
						clean_up(data);
						exit(data->prev_exit_stat);
					}
					else
					{
						data->mini_count--;
						return ;
					}
				}
			}
			if (args[2] != NULL)
			{
				ft_putstr_fd("exit: too many arguments\n", 2);
				data->prev_exit_stat = 1;
				return;
			}
			data->prev_exit_stat = exit_code;
		}
		else
		{
			data->prev_exit_stat = 0;
		}
		if (data->shlvl_history)
		 {
			int previous_shlvl = data->shlvl_history->shlvl_value;
			t_shlvl_node *temp = data->shlvl_history;
			data->shlvl_history = data->shlvl_history->next;
			free(temp);
			if (data->mini_count == 1)
			{
				clean_up(data);
				exit(data->prev_exit_stat);
			}
			else
			{
				data->mini_count--;
				if (previous_shlvl > 0)
				{
					char *new_value = ft_itoa(previous_shlvl);
					if (new_value)
					{
						update_env(&(data->env_list), "SHLVL", new_value);
						free(new_value);
					}
				}
				return;
			}
		}
		else
		{
			if (data->mini_count == 1)
			{
				clean_up(data);
				exit(data->prev_exit_stat);
			}
			else
			{
				data->mini_count--;
				return ;
			}
		}
	}
	else if (ft_strncmp(args[0], "pwd", ft_strlen("pwd")) == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			printf("%s\n", cwd);
		else
			perror("minishell");
	}
	else if (ft_strncmp(args[0], "echo", ft_strlen("echo")) == 0)
	{
		i = 1;
		newline = 1;
		str = NULL;
		if (!args[1] || !*(args[1]))
		{
			printf("\n");
			return ;
		}
		if (ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0)
		{
			newline = 0;
			i = 2;
		}
		str = join_space(&args[i]);
		if (newline)
			str = free_strjoin(str, "\n");
		printf("%s", str);
		free(str);
	}
	else if (ft_strncmp(args[0], "env", ft_strlen("env")) == 0)
	{
		print_env_list(data->env_list);
	}
	/* needs editing cause it shoudlnt be displayed in env if there is only name
	without value, it should be only displayed when export is called! ughhhhh */
	else if (ft_strncmp(args[0], "export", ft_strlen("export")) == 0)
	{
		execute_export(args, data);
	}
	else if (ft_strncmp(args[0], "unset", ft_strlen("unset")) == 0)
	{
		execute_unset(args, data);
	}
	/*
	OTHER CASES COME HERE ( ͡° ͜ʖ ͡° )
	*/
}

void clean_up(t_data *data)
{
	free_env_list(&(data->env_list));
	if (data->shlvl_history)
	{
		t_shlvl_node *current = data->shlvl_history;
		t_shlvl_node *next;
		while (current)
		{
			next = current->next;
			free(current);
			current = next;
		}
		data->shlvl_history = NULL;
	}
}
