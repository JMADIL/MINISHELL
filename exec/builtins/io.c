/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:43:20 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 14:28:45 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_input_redirection(t_redi_list *input_redir, int *input_fd)
{
	if (input_redir->type == INPUT)
	{
		if (input_redir->next)
			*input_fd = open(input_redir->next->file, O_RDONLY);
		if (*input_fd == -1)
		{
			perror(input_redir->file);
			return (-1);
		}
	}
	else if (input_redir->type == HEREDOC)
	{
		if (input_redir->heredoc_fd != -1)
			*input_fd = input_redir->heredoc_fd;
		else if (input_redir->tmp_fd != -1)
			*input_fd = input_redir->tmp_fd;
	}
	return (0);
}

static int	handle_output_redirection(t_redi_list *output_redir, int *output_fd,
		int *input_fd)
{
	if (output_redir->type == OUTPUT)
	{
		*output_fd = open(output_redir->file, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
		if (*output_fd == -1)
		{
			perror(output_redir->file);
			if (*input_fd != STDIN_FILENO)
				close(*input_fd);
			return (-1);
		}
	}
	else if (output_redir->type == APPEND)
	{
		*output_fd = open(output_redir->file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (*output_fd == -1)
		{
			perror(output_redir->file);
			if (*input_fd != STDIN_FILENO)
				close(*input_fd);
			return (-1);
		}
	}
	return (0);
}

int	setup_builtin_io(t_cmdarg *shell, int *input_fd, int *output_fd)
{
	t_redi_list	*redir;

	*input_fd = 0;
	*output_fd = 1;
	if (!shell->redirections)
		return (0);
	redir = shell->redirections;
	if (handle_input_redirection(redir, input_fd) == -1)
		return (-1);
	if (redir->type && 
		handle_output_redirection(redir, output_fd, input_fd) == -1)
		return (-1);
	return (0);
}
