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
 * @brief Prints the environment variables in the linked list
 */
void print_env_list(t_env *env)
{
	while (env != NULL)
	{
		if (env->value)
			printf("%s=%s\n", env->name, env->value);
		else
			printf("%s\n", env->name);
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