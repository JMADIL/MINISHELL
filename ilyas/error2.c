#include "help.h"

/*
 * Checks if a filename contains spaces that would make redirection ambiguous.
 * Detects cases where variable expansion or other operations result in
 * filenames containing spaces, which creates ambiguous redirection in shell.
 * Returns true for empty/null strings and strings containing spaces.
 *
 * @param file: Filename string to check for ambiguity
 * @return: true if filename is ambiguous (empty or contains spaces), false
 * otherwise
 */

bool is_ambi_filename(const char *file)
{
	if(file == NULL || *file == '\0')
		return true;
	if(ft_strchr(file, ' ') != NULL)
		return true;
	return false;
}


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
 // i will use this function in execution.c and its call sume of signals functions

}