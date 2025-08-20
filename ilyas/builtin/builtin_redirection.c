#include "../help.h"

/*
 * Handles append redirection for builtin commands
 * Opens file in append mode and redirects stdout if it's the last redirection
 */
int	handle_append_redi(t_redi_list *redi)
{
	int	fd;

	fd = open_file_for_builtin(redi->file, APPEND);
	if (fd == -1)
		return (0);
	redi->tmp_fd = fd;
	if (redi->is_last)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			return (0);
		}
		close(fd);
	}
	else
		close(fd);
	return (1);
}

/*
 * Handles regular output redirection for builtin commands
 * Opens file in write mode and redirects stdout if it's the last redirection
 */
int	handle_output_redi(t_redi_list *redi)
{
	int	fd;

	if (redi->type == OUTPUT)
	{
		fd = open_file_for_builtin(redi->file, OUTPUT);
		if (fd == -1)
			return (-1);
		redi->tmp_fd = fd;
		if (redi->is_last)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else
			close(fd);
	}
	else if (redi->type == APPEND)
	{
		if (!handle_append_redi(redi))
			return (-1);
	}
	return (1);
}

/*
 * Handles input redirection for builtin commands
 * Opens input file and redirects stdin
 */
int	handle_input_redi(t_redi_list *redi)
{
	int	fd;

	if (redi->type == INPUT)
	{
		fd = open_file_for_builtin(redi->file, INPUT);
		if (fd == -1)
			return (-1);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return (-1);
		}
		close(fd);
	}
	return (1);
}

/*
 * Sets up all redirections for builtin command execution
 * Saves original file descriptors and processes redirections
 */
int	setup_builtin_redirections(t_cmdarg *cmd)
{
	if (!cmd || !cmd->redirections)
		return (1);
	cmd->origin_stdin = dup(STDIN_FILENO);
	if (cmd->origin_stdin == -1)
	{
		print_error("dup stdin failed");
		g_exit_status = 1;
		return (1);
	}
	cmd->origin_stdout = dup(STDOUT_FILENO);
	if (cmd->origin_stdout == -1)
	{
		print_error("dup stdout failed");
		close(cmd->origin_stdin);
		g_exit_status = 1;
		return (1);
	}
	if (process_all_redirections(cmd->redirections) == -1)
		return (1);
	return (0);
}

/*
 * Main function to check if command is builtin and execute it
 * Handles single builtin commands with proper redirection setup
 */
int	execute_builtin_with_redi(t_cmdarg *cmd_list, t_list **env)
{
	int	builtin_check;

	if (!cmd_list || !cmd_list->cmd)
		return (0);
	builtin_check = check_if_builtin(cmd_list->cmd[0]);
	if (size_list(cmd_list) == 1 && builtin_check)
	{
		if (cmd_list->redirections)
		{
			if (setup_builtin_redirections(cmd_list) == 1)
			{
				restore_original_fds(cmd_list);
				return (1);
			}
		}
		if (execute_builtin_command(cmd_list, env) == 1)
		{
			if (cmd_list->redirections)
				restore_original_fds(cmd_list);
			return (1);
		}
		if (cmd_list->redirections)
			restore_original_fds(cmd_list);
		return (1);
	}
	return (0);
}
