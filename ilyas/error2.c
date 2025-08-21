#include "help.h"



/*
 * Completes execution process with proper signal handling and child waiting.
 * Sets up parent process signal handling for waiting state, waits for all
 * child processes to complete, then restores original signal handlers.
 * Used to properly finish command execution in pipeline.
 *
 * Side effects: Modifies signal handlers, waits for child processes
 */

void	finish_exec(pid_t last_cmd_pid)
{
	int	status;

	setup_parent_wait_signals();
	wait_for_all_children(&status, last_cmd_pid);
	restore_interactive_signals();
}