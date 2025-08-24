/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:37 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:10:45 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	}
	else if (redi->type == APPEND)
	{
		if (!handle_append_redi(redi))
			return (-1);
	}
	return (1);
}

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

int	setup_builtin_redirections(t_cmdarg *cmd)
{
	if (!cmd || !cmd->redirections)
		return (1);
	cmd->origin_stdin = dup(STDIN_FILENO);
	if (cmd->origin_stdin == -1)
	{
		perror("minishell: dup stdin");
		g_exit_status = 1;
		return (1);
	}
	cmd->origin_stdout = dup(STDOUT_FILENO);
	if (cmd->origin_stdout == -1)
	{
		perror("minishell: dup stdout");
		close(cmd->origin_stdin);
		g_exit_status = 1;
		return (1);
	}
	if (process_all_redirections(cmd->redirections) == -1)
		return (1);
	return (0);
}

int	execute_builtin_with_redi(t_cmdarg *cmd_list, t_list **env)
{
	int	builtin_check;

	if (!cmd_list || !cmd_list->cmd)
		return (0);
	builtin_check = is_builtin(cmd_list->cmd[0]);
	if (size_list(cmd_list) == 1 && builtin_check)
	{
		if (cmd_list->redirections && setup_builtin_redirections(cmd_list) == 1)
		{
			if (cmd_list->redirections)
				restore_original_fds(cmd_list);
			return (1);
		}
		if (exec_builtin(cmd_list, env) == 1)
		{
			if (cmd_list->redirections)
				restore_original_fds(cmd_list);
			return (1);
		}
	}
	return (0);
}
