#include "minishell.h"
#include <sys/stat.h>

void	free_strarray(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*join_path(const char *dir, const char *comm)
{
	char	*path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(comm) + 2; // +1 for '/' and +1 for '\0'
	path = malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, comm, len);
	return (path);
}

char	*find_slash(char *comm)
{
	if (ft_strchr(comm, '/'))
	{
		if (access(comm, X_OK) == 0)
			return (ft_strdup(comm));
		else
			return (NULL);
	}
	return (NULL);
}

// Function to search for the command in the PATH directories using access()
char	*find_exec(char *comm)
{
	char	*path_env;
	char	**dirs;
	char	*path;
	int		i;

	path = find_slash(comm);
	if (path)
		return (path);
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		path = join_path(dirs[i], comm);
		if (access(path, X_OK) == 0)
		{
			free_strarray(dirs);
			return (path);
		}
		free(path);
		i++;
	}
	free_strarray(dirs);
	return (NULL);
}

void	execute_command(char **args, t_data *data)
{
	pid_t	pid;
	int		status;
	char	*e_path;

	if (args[0] == NULL)
	{
		return ;
	}
	e_path = find_exec(args[0]);
	// printf("11e_path:%s\n",e_path);
	if (!e_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		// printf("TESTT\n");
		data->prev_exit_stat = 127;
		// printf("data->prev_exit_stat:%d\n", data->prev_exit_stat);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (execve(e_path, args, NULL) == -1)
		{
			perror("minishell");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		// Error forking
		perror("minishell");
	}
	else
	{
		// Parent process
		status = 0;
		waitpid(pid, &status, WUNTRACED);
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
		{
			waitpid(pid, &status, WUNTRACED);
		}
	}
	free(e_path);
}
