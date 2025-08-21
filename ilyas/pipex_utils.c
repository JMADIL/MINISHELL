#include "help.h"


//run_cmd_utils z3ma
/* ===========================================================
 * 1) Command-not-found / no-file error + exit(127)
 *    no_file == 1  -> "No such file or directory"
 *    no_file == 0  -> "command not found"
 * =========================================================== */
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

/* ===========================================================
 * 2) Heredoc -> stdin
 * =========================================================== */
void	handle_heredoc_input(t_redi_list *input)
{
	if (!input)
		return ;
	if (dup2(input->heredoc_fd, STDIN_FILENO) == -1)
		print_error_exit("dup2", "heredoc failed", 1);
	close(input->heredoc_fd);
}

/* ===========================================================
 * helpers: open file with error handling in child
 * mode: 0 => '>'  (O_WRONLY|O_CREAT|O_TRUNC)
 *       1 => '<'  (O_RDONLY)
 *       2 => '>>' (O_WRONLY|O_CREAT|O_APPEND)
 * =========================================================== */
static int	open_redir_file(const char *filename, int mode)
{
	int	fd;

	fd = safe_open(filename, mode);
	return (fd);
}

/* ===========================================================
 * 3) Handle >> append redirection for one node
 *     returns 1 on success
 * =========================================================== */
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
	input = input->next;
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

/* ===========================================================
 * 7) Malloc failure cleanup + exit
 * =========================================================== */
void	exec_malloc_fail(char *cmd_path, char *cmd_name)
{
	if (cmd_path)
		free(cmd_path);
	if (cmd_name)
		free(cmd_name);
	print_error_exit("malloc", "allocation failed", 1);
}
