#include "../minishell.h"

void	update_pwd(t_env **env_list)
{
	char cwd[1024];

	if (getcwd((cwd), sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	update_env(env_list, "PWD", cwd);
}

void	update_old_pwd(t_env **env_list)
{
	char cwd[1024];

	if (getcwd((cwd), sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	update_env(env_list, "OLDPWD", cwd);
}
