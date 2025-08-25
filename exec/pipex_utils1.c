/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:34:02 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 13:34:04 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cmd_not_found_exit(t_cmdarg *curr_cmd, int no_file)
{
	const char	*name;

	name = NULL;
	if (curr_cmd && curr_cmd->cmd && curr_cmd->cmd[0])
		name = curr_cmd->cmd[0];
	if (name == NULL)
		name = "(null)";
	if (no_file == 1)
		print_error_exit(name, "no such file or directory", 127);
	else
		print_error_exit(name, "comand not found", 127);
}

void	handle_heredoc_input(t_redi_list *input)
{
	int	fd;

	if (!input)
		return ;
	fd = create_tmp_heredoc();
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		print_error_exit("dup2", "heredoc failed", 1);
	}
	close(input->heredoc_fd);
}

int	open_redir_file(const char *filename, int mode)
{
	int	fd;

	fd = safe_open(filename, mode);
	return (fd);
}

int	handle_append_output(t_redi_list *output)
{
	int	fd;

	if (!output || !output->file)
		return (1);
	if (check_ambiguous_redirect(output->file))
		print_error_exit(output->file, "ambiguous redirect", 1);
	fd = safe_open(output->file, 2);
	if (output->is_last == true)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			print_error_exit("dup2", "append redirection failed", 1);
		}
		close(fd);
	}
	return (1);
}

void	exec_malloc_fail(char *cmd_path, char *cmd_name)
{
	if (cmd_path)
		free(cmd_path);
	if (cmd_name)
		free(cmd_name);
	print_error_exit("malloc", "allocation failed", 1);
}
