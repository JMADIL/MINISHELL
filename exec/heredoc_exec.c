#include "../minishell.h"

/* ============================================================================
 * 1) Child-side: read heredoc until delimiter with GNL; write to pipe and exit
 *    - sets heredoc-specific signals
 *    - writes into fd_pipe[1]
 *    - closes write-end and _exit() (does not return)

	* ============================================================================ */
int	heredoc_child_process(t_redi_list *heredoc, int fd_pipe[2], t_list *env)
{
	char	*delim;
	delim = heredoc->delim;//nxofo leiba

	if (!heredoc)
		_exit(1);
	setup_heredoc_signals();
	if (fd_pipe[0] >= 0)
		close(fd_pipe[0]);
	read_heredoc_input_gnl(delim, fd_pipe, heredoc, env);
	if (fd_pipe[1] >= 0)
		close(fd_pipe[1]);
	_exit(g_exit_status);
}

/* ============================================================================
 * 2) Parent-side: close write-end, wait child, store read-end FD if last
 *    - closes write-end
 *    - waitpid on child, propagate g_exit_status
 *    - if this heredoc is the "last" for the command, keep read-end in node;
 *      otherwise close it

	* ============================================================================ */
void	heredoc_parent_finalize(int fd_pipe[2], pid_t pid, int *status,
		t_redi_list *in)
{
	close(fd_pipe[1]);
	setup_parent_heredoc_signals();
	if (waitpid(pid, status, 0) == -1)
	{
		close(fd_pipe[0]);
		g_exit_status = 1;
		restore_interactive_signals();
		return ;
	}
	if (WIFSIGNALED(*status))
		g_exit_status = 128 + WTERMSIG(*status);
	else
		g_exit_status = WEXITSTATUS(*status);
	if (in && in->is_last && g_exit_status == 0)
		in->heredoc_fd = fd_pipe[0];
	else
		close(fd_pipe[0]);
	restore_interactive_signals();
}

/* ============================================================================
 * 3) Handle a single heredoc redirection:
 *    - create pipe
 *    - fork child to read with GNL
 *    - parent waits and stores read-end if needed
 *    Returns: 1 on success, -1 on failure

	* ============================================================================ */
int	handle_single_heredoc(t_redi_list *in, int fd_pipe[2], t_list *env)
{
	pid_t	pid;
	int		status;

	if (!in || in->type != HEREDOC)
		return (1);
	if (pipe(fd_pipe) == -1)
		print_error_exit("pipe", "pipe creation failed", 1);
	pid = fork();
	if (pid == -1)
	{
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		print_error_exit("fork", "fork failed", 1);
	}
	if (pid == 0)
		heredoc_child_process(in, fd_pipe, env);
	heredoc_parent_finalize(fd_pipe, pid, &status, in);
	if (g_exit_status != 0)
		return (-1);
	return (1);
}

/* ============================================================================
 * 4) Process all heredocs for the whole pipeline:
 *    - init redirection metadata (marks last nodes)
 *    - for each command node, handle each HEREDOC redirection
 *    Returns: 1 on success, 0 on failure/interruption

	* ============================================================================ */
int	process_all_heredocs(t_cmdarg *shell, t_list *env)
{
	t_cmdarg	*cmd;
	t_redi_list	*r;
	int			res;

	if (!shell)
		return (1);
	init_redirection_metadata(shell);
	cmd = shell;
	while (cmd)
	{
		r = cmd->redirections;
		while (r)
		{
			if (r->type == HEREDOC)
			{
				int a[2];
				res = handle_single_heredoc(r, a, env);
				if (res == -1)
					return (0);
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (1);
}
