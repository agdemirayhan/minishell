#include "../minishell.h"

void	print_cmds(t_list *cmds)
{
	t_list	*current;
	t_mini	*cmd_data;
	int		cmd_index;

	cmd_index = 0;
	current = cmds;
	while (current != NULL)
	{
		cmd_data = (t_mini *)current->content;
		printf("Command %d:\n", cmd_index);
		// Print infile and outfile
		printf("\tinfile: %d\n", cmd_data->infile);
		printf("\toutfile: %d\n", cmd_data->outfile);
		// Print full_path (if applicable)
		if (cmd_data->full_path)
			printf("\tfull_path: %s\n", cmd_data->full_path);
		else
			printf("\tfull_path: (builtin or not set)\n");
		// Print the full command
		if (cmd_data->full_cmd)
		{
			printf("\tfull_cmd: {");
			for (int i = 0; cmd_data->full_cmd[i] != NULL; i++)
			{
				printf("%s", cmd_data->full_cmd[i]);
				if (cmd_data->full_cmd[i + 1] != NULL)
					printf(", ");
			}
			printf("}\n");
		}
		else
		{
			printf("\tfull_cmd: (none)\n");
		}
		// Move to the next command
		current = current->next;
		cmd_index++;
	}
}
