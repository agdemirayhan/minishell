#include "minishell.h"

// Function Declarations
char	*expand_env_vars(char *input, t_data *data);
size_t	calculate_total_size(char *input, t_data *data);
size_t	expand_variables(char *input, char *result, t_data *data,
			size_t total_size);
int		handle_single_quote(char *input, size_t *i, char *result,
			size_t *result_len, int in_single_quote);
size_t	get_exit_status_size(t_data *data);
size_t	expand_exit_status(t_data *data, char *result, size_t result_len,
			size_t total_size);
size_t	expand_variable(char *input, size_t *i, t_data *data, char *result,
			size_t result_len, size_t total_size);
char	*extract_var_name(char *input, size_t *i);
size_t	copy_to_result(char *result, char *src, size_t result_len,
			size_t total_size);
size_t	get_var_size(char *input, size_t *i, t_data *data);

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
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}

char	*expand_env_vars(char *input, t_data *data)
{
	size_t	total_size;
	size_t	result_len;
	char	*result;

	total_size = calculate_total_size(input, data);
	result = malloc(total_size);
	if (!result)
		return (NULL);
	result_len = expand_variables(input, result, data, total_size);
	result[result_len] = '\0';
	return (result);
}

size_t	calculate_total_size(char *input, t_data *data)
{
	size_t	total_size;
	size_t	i;
	int		in_single_quote;

	total_size = 1;
	i = 0;
	in_single_quote = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'')
			in_single_quote = !in_single_quote;
		else if (input[i] == '$' && input[i + 1] != '\0' && !in_single_quote)
		{
			i++;
			if (input[i] == '?')
				total_size += get_exit_status_size(data);
			else
				total_size += get_var_size(input, &i, data);
		}
		else
			total_size++;
		i++;
	}
	return (total_size);
}

size_t	expand_variables(char *input, char *result, t_data *data,
		size_t total_size)
{
	size_t	i;
	size_t	result_len;
	int		in_single_quote;

	i = 0;
	result_len = 0;
	in_single_quote = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'')
			in_single_quote = handle_single_quote(input, &i, result,
					&result_len, in_single_quote);
		else if (input[i] == '$' && input[i + 1] != '\0' && !in_single_quote)
		{
			i++;
			if (input[i] == '?')
				result_len = expand_exit_status(data, result, result_len,
						total_size);
			else
				result_len = expand_variable(input, &i, data, result,
						result_len, total_size);
		}
		else
			result[result_len++] = input[i++];
	}
	return (result_len);
}

int	handle_single_quote(char *input, size_t *i, char *result,
		size_t *result_len, int in_single_quote)
{
	result[*result_len] = input[*i];
	(*result_len)++;
	(*i)++;
	return (!in_single_quote);
}

size_t	get_exit_status_size(t_data *data)
{
	char	*exit_stat;
	size_t	exit_stat_len;

	exit_stat = ft_itoa(data->prev_exit_stat);
	exit_stat_len = ft_strlen(exit_stat);
	free(exit_stat);
	return (exit_stat_len);
}

size_t	expand_exit_status(t_data *data, char *result, size_t result_len,
		size_t total_size)
{
	char	*exit_stat;
	size_t	exit_stat_len;

	exit_stat = ft_itoa(data->prev_exit_stat);
	exit_stat_len = ft_strlen(exit_stat);
	if (result_len + exit_stat_len < total_size)
		ft_strlcpy(result + result_len, exit_stat, total_size - result_len);
	result_len += exit_stat_len;
	free(exit_stat);
	return (result_len);
}

size_t	get_var_size(char *input, size_t *i, t_data *data)
{
	char	*var_name;
	char	*var_value;
	size_t	start;
	size_t	size;

	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(input + start, *i - start);
	var_value = find_env_ref(data->env_list, var_name);
	size = var_value ? ft_strlen(var_value) : (*i - start);
	free(var_name);
	return (size);
}

size_t	expand_variable(char *input, size_t *i, t_data *data, char *result,
		size_t result_len, size_t total_size)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name(input, i);
	var_value = find_env_ref(data->env_list, var_name);
	if (var_value)
		result_len = copy_to_result(result, var_value, result_len, total_size);
	else
		result_len = copy_to_result(result, var_name, result_len, total_size);
	free(var_name);
	return (result_len);
}

char	*extract_var_name(char *input, size_t *i)
{
	size_t	start;
	char	*var_name;

	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(input + start, *i - start);
	return (var_name);
}

size_t	copy_to_result(char *result, char *src, size_t result_len,
		size_t total_size)
{
	ft_strlcpy(result + result_len, src, total_size - result_len);
	return (result_len + ft_strlen(src));
}
