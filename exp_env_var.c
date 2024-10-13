#include "minishell.h"

/**
 * @brief searches for the env variable
 * @param env_list part of the struct which contains the env vars
 * @param name name of the env variable
 */
void	*find_env_ref(t_env *env_list, char *name)
{
	t_env	*curr;

	curr = env_list;
	while (curr)
	{
		if (ft_strncmp(curr->name, name, ft_strlen(name)) == 0)
		{
			return (curr->value);
		}
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief expands env vars found in the input string
 * @param input the string containing env vars to expand
 * @param data struct containing the env list

	* @return a newly allocated string with all env vars expanded. Memory should be freed after the function is called!!!
 */
char	*expand_env_vars(char *input, t_data *data)
{
	size_t total_size;
	size_t i;
	size_t start;
	char *var_name;
	char *var_value;
	char *result;
	size_t result_len;
	char *exit_stat;
	int in_single_quote;

	total_size = 1;
	i = 0;
	in_single_quote = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'')
		{
			in_single_quote = !in_single_quote;
			total_size++;
			i++;
		}
		else if (input[i] == '$' && input[i + 1] != '\0' && !in_single_quote)
		{
			i++;
			if (input[i] == '?')
			{
				exit_stat = ft_itoa(data->prev_exit_stat);
				total_size += ft_strlen(exit_stat);
				i++;
			}
			else
			{
				start = i;
				while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				{
					i++;
				}
				var_name = ft_strndup(input + start, i - start);
				var_value = find_env_ref(data->env_list, var_name);
				if (var_value)
					total_size += ft_strlen(var_value);
				else
					total_size += i - start;
				free(var_name);
			}
		}
		else
		{
			total_size++;
			i++;
		}
	}
	result = malloc(total_size);
	if (!result)
		return (NULL);
	result_len = 0;
	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'')
		{
			in_single_quote = !in_single_quote;
			result[result_len++] = input[i++];
		}
		else if (input[i] == '$' && input[i + 1] != '\0' && !in_single_quote)
		{
			i++;
			if (input[i] == '?')
			{
				ft_strlcpy(result + result_len, exit_stat, total_size
					- result_len);
				result_len += ft_strlen(exit_stat);
				i++;
			}
			else
			{
				start = i;
				while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				{
					i++;
				}
				var_name = ft_strndup(input + start, i - start);
				var_value = find_env_ref(data->env_list, var_name);
				if (var_value)
				{
					ft_strlcpy(result + result_len, var_value, total_size
						- result_len);
					result_len += ft_strlen(var_value);
				}
				else
				{
					ft_strlcpy(result + result_len, var_name, total_size
						- result_len);
					result_len += ft_strlen(var_name);
				}
				free(var_name);
			}
		}
		else
		{
			result[result_len++] = input[i++];
		}
	}
	result[result_len] = '\0';
	return (result);
}