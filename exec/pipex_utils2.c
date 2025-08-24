/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:16:45 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 04:59:30 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* ===========================================================
 * Helper function for handling > redirection
 * =========================================================== */
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

/* ===========================================================
 * 4) Handle all OUTPUT redirections (> and >>)
 *     earlier ones: open/close only
 *     last one: dup2(STDOUT_FILENO)
 * =========================================================== */
void	process_output_redirections(t_redi_list *output)
{
	t_redi_list	*node;

	node = output;
	// if (!node->type)
	// 	return ;
	while (node)
	{
		if (node->type == OUTPUT) {
			handle_output_redirect(node);
		}
		else if (node->type == APPEND) {
			handle_append_output(node);
		}
		node = node->next;
	}
}

/* ===========================================================
 * 5) Handle all INPUT redirections (< and <<)
 *     earlier ones: open/close only
 *     last one: dup2(STDIN_FILENO)
 * =========================================================== */
void	process_input_redirections(t_redi_list *input)
{
	int	fd;
	t_redi_list	*node;

	if (!input)
		return ;

	node = input;
	while (node)
	{
		if (node->type == INPUT)
		{
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
		else if (node->type == HEREDOC)
		{
			if (node->is_last == true)
				handle_heredoc_input(input);		
			else if (node->heredoc_fd >= 0)
				close(node->heredoc_fd);
		}
		node = node->next;
	}
}

/* ===========================================================
 * 6) Validate absolute/relative path is executable
 *     returns dup of p if executable and not a directory
 * =========================================================== */
char	*validate_exec_path(char *p)
{
	if (!p || p[0] == '\0')
		return (NULL);
	if (p[0] == '/' || p[0] == '.')
	{
		if (access(p, X_OK) == 0)
			return (ft_strdup(p));
	}
	return (NULL);
}
