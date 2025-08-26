/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:16:45 by irfei             #+#    #+#             */
/*   Updated: 2025/08/26 05:26:51 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_output_redirect(t_redi_list *node)
{
	int	fd;

	if (!node || !node->file)
		return (1);
	fd = safe_open(node->file, 0);
	if (node->is_last == true)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			print_error_exit("dup2", "output redirection failed", 1);
		}
	}
	close(fd);
	return (1);
}

void	process_output_redirections(t_redi_list *output)
{
	t_redi_list	*node;

	node = output;
	while (node)
	{
		if (node->type == OUTPUT)
		{
			handle_output_redirect(node);
		}
		else if (node->type == APPEND)
		{
			handle_append_output(node);
		}
		node = node->next;
	}
}

static void	handle_input_redirection(t_redi_list *node)
{
	int	fd;

	fd = open_redir_file(node->file, 1);
	if (node->is_last == true)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			print_error_exit("dup2", "input redirection failed", 1);
		}
	}
	close(fd);
}

static void	handle_heredoc_redirection(t_redi_list *node, t_redi_list *input)
{
	if (node->is_last == true)
		handle_heredoc_input(input);
	else if (node->heredoc_fd >= 0)
		close(node->heredoc_fd);
}

void	process_input_redirections(t_redi_list *input)
{
	t_redi_list	*node;

	if (!input)
		return ;
	node = input;
	while (node)
	{
		if (node->type == INPUT)
			handle_input_redirection(node);
		else if (node->type == HEREDOC)
			handle_heredoc_redirection(node, input);
		node = node->next;
	}
}
