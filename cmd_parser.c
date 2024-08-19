#include "minishell.h"

void	parse_command(char *input)
{
	char	*args[1024];
	//char	*temp;
	int		i;
	//int		start;
	//int		end;

	i = 0;
	//start = 0;
	args[i] = strtok(input, " ");	//strtok not allowed
	while (args[i] != NULL)
	{
		i++;
		args[i] = strtok(NULL, " ");
	}
	//while (input[start] != '\0')
	//{
	//	end = ft_strchr(input + start, ' ') - input;
	//	if (end < start)
	//		end = ft_strlen(input);
	//	temp = ft_substr(input, start, end - start);
	//	if (temp)
	//		args[i++] = temp;
	//	if (input[end] == '\0')
	//		break ;
	//	start = end + 1;
	//}
	//args[i] = NULL;
	if (args[0] == NULL)
		return ;
	if (is_builtin(args[0]))
	{
		execute_builtin(args);
	}
	else
	{
		execute_command(args);
	}
}
