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

// char *find_executable(char *command)
//{
//	char	*path_env = getenv("PATH");
//	char	**paths;
//	char	*full_path;
//	struct stat buffer;
//	int i = 0;

//	if (!path_env)
//		return (NULL);

//	// Split the PATH variable into directories
//	paths = ft_split(path_env, ':');
//	if (!paths)
//		return (NULL);

//	// Loop through each directory in PATH
//	while (paths[i])
//	{
//		full_path = ft_strjoin(paths[i], "/");
//		full_path = free_strjoin(full_path, command);
//		// Check if the file exists and is executable
//		if (stat(full_path, &buffer) == 0 && (buffer.st_mode & S_IXUSR))
//		{
//			free_strarray(paths); // Free the split array
//			return (full_path);
//		}
//		free(full_path); // Free full_path for this iteration
//		i++;
//	}
//	free_strarray(paths); // Free the split array
//	return (NULL); // Return NULL if command not found
//}

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
			return (free_strarray(dirs), path);
		free(path);
		i++;
	}
	return (free_strarray(dirs), NULL);
}

void	handle_fork_and_execute(char *e_path, char **args)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(e_path, args, NULL) == -1)
			perror("minishell");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("minishell");
	else
	{
		status = 0;
		waitpid(pid, &status, WUNTRACED);
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
			waitpid(pid, &status, WUNTRACED);
	}
}

void	execute_command(char **args)
{
	char	*e_path;

	if (args[0] == NULL)
		return ;
	e_path = find_exec(args[0]);
	if (!e_path)
	{
		printf("minishell: command not found: %s\n", args[0]);
		return ;
	}
	handle_fork_and_execute(e_path, args);
	free(e_path);
}
