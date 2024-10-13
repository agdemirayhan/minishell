#include "minishell.h"

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	len;
	char	*str;

	len = ft_strlen(s1);
	if (len > n)
		len = n;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str = ft_memcpy(str, s1, len);
	str[len] = '\0';
	return (str);
}

/**
 * @brief Initializes and returns a linked list of environment variables from the given array.
 * @return A pointer to the head of the linked list, or NULL if allocation fails.
 */
t_env *init_env_list(char **envp)
{
	t_env	*head;
	t_env	*curr;
	t_env	*new_node;
	char	*equal;
	int		i;

	head = NULL;
	curr = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			new_node->name = ft_strndup(envp[i], equal - envp[i]);
			new_node->value = ft_strdup(equal + 1);
		}
		else
		{
			new_node->name = ft_strdup(envp[i]);
			new_node->value = NULL;
		}
		if (ft_strncmp(new_node->name, "SHLVL", 6) == 0) //shlvl setter
		{
			free(new_node->value);
			new_node->value = ft_strdup("2");
		}
		if (ft_strncmp(new_node->name, "_", 1) == 0)
		{
			free(new_node->value);
			new_node->value = ft_strdup("/usr/bin/env");
		}
		new_node->next = NULL;
		if (head == NULL)
		{
			head = new_node;
			curr = head;
		}
		else
		{
			curr->next = new_node;
			curr = curr->next;
		}
		i++;
	}
	return (head);
}
/**
 * @brief updates the value of the env variables
 */
void	update_env(t_env **env_list, char *name, char *value)
{
	t_env	*temp;
	//t_env	*new_node;

	temp = *env_list;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->name, name, ft_strlen(name)) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			return ;
		}
		temp = temp->next;
	}
	//new_node = malloc(sizeof(t_env));
	//if (!new_node)
	//	return ;
	//new_node->name = ft_strdup(name);
	//new_node->value = ft_strdup(value);
	//new_node->next = *env_list;
	//*env_list = new_node;
}

/**
 * @brief Prints the environment variables in the linked list
 */
void print_env_list(t_env *env)
{
	while (env != NULL)
	{
		if (ft_strncmp(env->name, "_", 1) == 0 && !(ft_strncmp(env->name, "__", 2) == 0))
			printf("_=/usr/bin/env\n");
		else
		{
			if (env->value)
				printf("%s=%s\n", env->name, env->value);
			else
				printf("%s\n", env->name);
		}
		env = env->next;
	}
}

void free_env_list(t_env **env)
{
	t_env	*current;
	t_env	*next;

	current = *env;
	while (current != NULL)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}