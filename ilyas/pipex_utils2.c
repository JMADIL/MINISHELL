
#include "help.h"

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
int	process_output_redirections(t_redi_list *output)
{
	t_redi_list	*node;

	node = output;
	while (node)
	{
		if (node->type == OUTPUT)
			handle_output_redirect(node);
		else if (node->type == APPEND)
			handle_append_output(node);
		node = node->next;
	}
	return (1);
}

/* ===========================================================
 * 5) Handle all INPUT redirections (< and <<)
 *     earlier ones: open/close only
 *     last one: dup2(STDIN_FILENO)
 * =========================================================== */
int	process_input_redirections(t_redi_list *input)
{
	int	fd;

	if (input->type == INPUT)
	{
		if (check_ambiguous_redirect(input->file))
			print_error_exit(input->file, "ambiguous redirect", 1);
		fd = open_redir_file(input->file, 1);
		if (input->is_last == true)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				close(fd);
				print_error_exit("dup2", "input redirection failed", 1);
			}
		}
		close(fd);
	}
	else if (input->type == HEREDOC)
	{
		if (input->is_last == true)
			handle_heredoc_input(input);
		else if (input->heredoc_fd >= 0)
			close(input->heredoc_fd);
	}
	return (input = input->next, 0);
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
