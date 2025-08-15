#include "../help.h"


/*
 * Opens file for builtin redirection based on type
 * Returns file descriptor or -1 on error
 */
int	open_file_for_builtin(char *filename, t_token_type type)
{
	int	fd;

	if (!filename || filename[0] == '\0')
	{
		display_redi_error(filename, "No such file or directory");
		return (-1);
	}
	
	if (check_ambiguous_redirect(filename))
	{
		display_redi_error(filename, "ambiguous redirect");
		return (-1);
	}
	
	if (type == OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == INPUT)
		fd = open(filename, O_RDONLY);
	else if (type == APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	
	if (fd == -1)
		handle_file_open_error(filename);
	
	return (fd);
}

/*
 * Restores original stdin and stdout after builtin execution
 */
void	restore_original_fds(t_cmdarg *cmd)
{
	if (cmd->origin_stdout != -1)
	{
		if (dup2(cmd->origin_stdout, STDOUT_FILENO) == -1)
			write(STDERR_FILENO, "Error: failed to restore stdout\n", 33);
		close(cmd->origin_stdout);
		cmd->origin_stdout = -1;
	}
	
	if (cmd->origin_stdin != -1)
	{
		if (dup2(cmd->origin_stdin, STDIN_FILENO) == -1)
			write(STDERR_FILENO, "Error: failed to restore stdin\n", 32);
		close(cmd->origin_stdin);
		cmd->origin_stdin = -1;
	}
}

/*
 * Displays redirection error in minishell format
 */
void	display_redi_error(char *filename, char *error_msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (filename)
	{
		write(STDERR_FILENO, filename, ft_strlen(filename));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	write(STDERR_FILENO, "\n", 1);
	g_exit_status = 1;
}

/*
 * Handles file opening errors and displays appropriate message
 */
void	handle_file_open_error(char *filename)
{
	if (errno == ENOENT)
		display_redi_error(filename, "No such file or directory");
	else if (errno == EISDIR)
		display_redi_error(filename, "Is a directory");
	else if (errno == EACCES)
		display_redi_error(filename, "Permission denied");
	else
		display_redi_error(filename, "Cannot open file");
}

/*
 * Processes all redirections in the list
 */
int	process_all_redirections(t_redi_list *redi_list)
{
	t_redi_list	*current;

	current = redi_list;
	while (current)
	{
		if (current->type == INPUT)
		{
			if (handle_input_redi(current) == -1)
				return (-1);
		}
		else if (current->type == OUTPUT || current->type == APPEND)
		{
			if (handle_output_redi(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (1);
}

/*
 * Checks if redirect target is ambiguous
 */
bool	check_ambiguous_redirect(char *filename)
{
	// Add your ambiguous redirect logic here
	// For now, simple check
	if (!filename || ft_strlen(filename) == 0)
		return (true);
	return (false);
}

/*
 * Counts number of commands in pipeline
 */
int	count_commands(t_cmdarg *cmd_list)
{
	int			count;
	t_cmdarg	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
 * Checks if command is a builtin
 */
int	check_if_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	
	if (strcmp(cmd, "echo") == 0)
		return (1);
	if (strcmp(cmd, "cd") == 0)
		return (1);
	if (strcmp(cmd, "pwd") == 0)
		return (1);
	if (strcmp(cmd, "export") == 0)
		return (1);
	if (strcmp(cmd, "unset") == 0)
		return (1);
	if (strcmp(cmd, "env") == 0)
		return (1);
	if (strcmp(cmd, "exit") == 0)
		return (1);
	
	return (0);
}

/*
 * Executes builtin command
 */
int	execute_builtin_command(t_cmdarg *cmd, t_list **env)
{
	if (!cmd || !cmd->cmd || !cmd->cmd[0])
		return (0);
	
	if (strcmp(cmd->cmd[0], "echo") == 0)
		return (builtin_echo(cmd->cmd));
	// Add other builtin executions here
	
	return (0);
}

/*
 * Simple error printing function
 */
void	print_error(char *msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}