#include "../minishell.h"

/* ===========================================================
SIGINT handler for heredoc child process
 * =========================================================== */
void	sigint_heredoc_child(int sig)
{
	(void)sig;
	_exit(1);
}

/* ===========================================================
 Setup default signal handlers for interactive shell
 * =========================================================== */
void	setup_interactive_signals(void)
{
	signal(SIGINT, sigint_interactive);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
Setup signal handlers for heredoc input collection
 * =========================================================== */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
Restore default interactive shell signal handlers
 * =========================================================== */
void	restore_interactive_signals(void)
{
	signal(SIGINT, sigint_interactive);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
Setup default signal handlers for child processes
 * =========================================================== */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}