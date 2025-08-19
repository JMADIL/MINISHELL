#include "help.h"

/* ===========================================================
 * 1) SIGINT handler for interactive prompt
 * =========================================================== */
void	sigint_interactive(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 1;
}

/* ===========================================================
 * 2) SIGINT handler while parent waits for child execution
 * =========================================================== */
void	sigint_parent_wait(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 130;
}

/* ===========================================================
 * 3) SIGINT handler for heredoc input collection
 * =========================================================== */
void	sigint_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 1;
}

/* ===========================================================
 * 4) SIGINT handler for heredoc child process
 * =========================================================== */
void	sigint_heredoc_child(int sig)
{
	(void)sig;
	_exit(1);
}

/* ===========================================================
 * 5) Setup default signal handlers for interactive shell
 * =========================================================== */
void	setup_interactive_signals(void)
{
	signal(SIGINT, sigint_interactive);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
 * 6) Setup signal handlers for heredoc input collection
 * =========================================================== */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
 * 7) Restore default interactive shell signal handlers
 * =========================================================== */
void	restore_interactive_signals(void)
{
	signal(SIGINT, sigint_interactive);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
 * 8) Setup default signal handlers for child processes
 * =========================================================== */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/* ===========================================================
 * 9) Setup signal handlers for parent while waiting for children
 * =========================================================== */
void	setup_parent_wait_signals(void)
{
	signal(SIGINT, sigint_parent_wait);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
 * 10) Setup signal handlers for parent during heredoc processing
 * =========================================================== */
void	setup_parent_heredoc_signals(void)
{
	signal(SIGINT, sigint_heredoc_child);
	signal(SIGQUIT, SIG_IGN);
}
