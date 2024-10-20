/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:43:23 by aagdemir          #+#    #+#             */
/*   Updated: 2024/10/20 23:57:39 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_path(const char *dir, const char *comm)
{
	char	*path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(comm) + 2;
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
	free_strarray(dirs);
	return (NULL);
}

void	wait_for_process(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, WUNTRACED);
	while (!WIFEXITED(status) && !WIFSIGNALED(status))
	{
		waitpid(pid, &status, WUNTRACED);
	}
}

void	execute_command(char **args, t_data *data)
{
	pid_t	pid;
	char	*e_path;

	if (args[0] == NULL)
		return ;
	e_path = find_exec(args[0]);
	if (!e_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		data->prev_exit_stat = 127;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(e_path, args, NULL) == -1)
			perror("minishell");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("minishell");
	free(e_path);
	wait_for_process(pid);
}
