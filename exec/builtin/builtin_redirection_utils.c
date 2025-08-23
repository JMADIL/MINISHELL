/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:31 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 22:05:33 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
