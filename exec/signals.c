#include "../minishell.h"

/* ===========================================================
 SIGINT handler for interactive prompt
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
SIGINT handler while parent waits for child execution
 * =========================================================== */
void	sigint_parent_wait(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 130;
}

/* ===========================================================
 SIGINT handler for heredoc input collection
 * =========================================================== */
void	sigint_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 1;
}



/* ===========================================================
 Setup signal handlers for parent while waiting for children
 * =========================================================== */
void	setup_parent_wait_signals(void)
{
	signal(SIGINT, sigint_parent_wait);
	signal(SIGQUIT, SIG_IGN);
}

/* ===========================================================
 Setup signal handlers for parent during heredoc processing
 * =========================================================== */
void	setup_parent_heredoc_signals(void)
{
	// signal(SIGINT, sigint_heredoc_child);
	signal(SIGQUIT, SIG_IGN);
}
