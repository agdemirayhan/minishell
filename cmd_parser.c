#include "minishell.h"


int	is_redirection(char *arg)
{
	if (!arg)
		return (0);                  // Null argument is not a redirection
	if (ft_strcmp(arg, ">") == 0 ||  // Output redirection
		ft_strcmp(arg, ">>") == 0 || // Append output redirection
		ft_strcmp(arg, "<") == 0 ||  // Input redirection
		ft_strcmp(arg, "<<") == 0)   // Here-document redirection
	{
		return (1); // It is a redirection operator
	}
	return (0); // Not a redirection operator
}



/**
 * @brief Creates linked lists of commands split by pipes ('|').
 * Parses the input `args` array and creates separate linked
 * lists for each set of commands
 * and arguments divided by pipes. Returns an array of linked lists.
 * @param args Array of strings representing the parsed command input.
 * @return An array of linked lists,
 * each representing a command chain. The array ends with NULL.
 */

void	parse_command(char *input, t_data *data)
{
	t_prompt	test;
	t_list		*cmd_node;
	t_mini		*mini_cmd;

	parse_command2(input, data, &test);
	if (test.cmds && test.cmds->next != NULL)
		execute_pipes(test.cmds, data);
	else
	{
		cmd_node = test.cmds;
		while (cmd_node)
		{
			mini_cmd = (t_mini *)cmd_node->content;
			if (mini_cmd && mini_cmd->full_cmd && mini_cmd->full_cmd[0])
			{
				execute_mini_command(mini_cmd, data);
				check_and_update_shlvl(data, mini_cmd);
			}
			cmd_node = cmd_node->next;
		}
	}
}
