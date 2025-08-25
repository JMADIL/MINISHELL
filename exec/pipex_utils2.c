/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:16:45 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 14:19:21 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_output_redirect(t_redi_list *node)
{
	int	fd;

	if (!node || !node->file)
		return (1);
	if (check_ambiguous_redirect(node->file))
		print_error_exit(node->file, "ambiguous redirect", 1);
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

	if (check_ambiguous_redirect(node->file))
		print_error_exit(node->file, "ambiguous redirect", 1);
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
