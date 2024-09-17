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
	size_t total_length = 0;
	int i = 0;

	while (args[i] != NULL)
	{
		total_length += ft_strlen(args[i]) + 1;
		i++;
	}
	char *result = malloc(total_length);
	if (result == NULL)
		return NULL;
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
	char *result;

	new_length = ft_strlen(str1) + ft_strlen(str2) + 1;
	result = malloc(new_length);
	if (result == NULL)
	{
		free(str1);
		return NULL;
	}
	ft_strlcpy(result, str1, new_length);
	ft_strlcat(result, str2, new_length);
	free(str1);
	return result;
}

char *get_dir(char **args)
{
	char *home;
	char *home1;

	if (!args[1] || ft_strlen(args[1]) == 0)
	{
		home = getenv("HOME");
		if (!home)
			return NULL;
		return ft_strdup(home);
	}
	if (ft_strncmp(args[1], "~", 1) == 0)
	{
		home1 = getenv("HOME");
		if (!home1)
			return (NULL);
		return ft_strjoin(home1, args[1] + 1);
	}
	return (ft_strdup(args[1]));
}


void	execute_builtin(char **args, t_data *data)
{
	char	cwd[1024];
	int		i;

	// what happens when someone tries it with a directory which requires more than 1024 chars????? fails. using malloc for this?? eww. <--this is actually ok
	// also it needs to handle cd ~ so it can go home ewwwwww.
	if (ft_strncmp(args[0], "cd", ft_strlen("cd")) == 0)
	{
		char	*dir;

		//if (chdir(args[1]) != 0)
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
	//this probably needs fixing too
	else if (ft_strncmp(args[0], "exit", ft_strlen("exit")) == 0)
	{
		exit(0);
	}
	// with getenv pwd output was always the same. therefore it was changed with getcwd, still doesnt change in the env :(
	else if (ft_strncmp(args[0], "pwd", ft_strlen("pwd")) == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			printf("%s\n", cwd);
		else
			perror("minishell");
	}
	//need to deal with quoted echoes
	else if (ft_strncmp(args[0], "echo", ft_strlen("echo")) == 0)
	{
		int i = 1;
		int newline = 1;
		char *str = NULL;
		if (!args[1] || !*(args[1]))
		{
			printf("\n");
			return;
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
