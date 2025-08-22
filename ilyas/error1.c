#include "help.h"

/*
 * Safely frees a character pointer and sets it to NULL.
 * Provides null-safe string deallocation by checking for null pointer
 * before freeing. Sets pointer to NULL after freeing to prevent
 * double-free errors and dangling pointer issues.
 *
 * @param s: Character pointer to free
 * Side effects: Frees memory if pointer is not NULL, sets pointer to NULL
 */

void	safe_free(char *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}
/*
 * Prints error message and exits with specified status code.
 * Formats and displays shell error messages with consistent format:
 * "minishell : [command]: [error]". Terminates the current process
 * with the provided exit status after printing the error.
 *
 * @param cmd_name: Command name to include in error message (can be NULL)
 * @param error: Error message string to display
 * @param status: Exit status code for process termination
 * Side effects: Writes error to stderr, terminates process
 */
void	print_error_exit(const char *cmd_name, const char *error, int status)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (cmd_name)
	{
		write(STDERR_FILENO, cmd_name, ft_strlen((char *)cmd_name));
		write(STDERR_FILENO, ": ", 2);
	}
	if (error)
		write(STDERR_FILENO, error, ft_strlen((char *)error));
	write(STDERR_FILENO, "\n", 1);
	_exit(status);
}

/*
 * Opens files with specified flags for redirection operations.
 * Handles different file opening modes: truncate (flag 0), read-only (flag 1),
 * and append (flag 2). Reports appropriate error messages for common
 * failure cases like file not found, directory access, and permission issues.
 *
 * @param file: Filename to open
 * @param flag: Opening mode (0=truncate, 1=read, 2=append)
 * @return: File descriptor on success, -1 on failure (with error reported)
 * Side effects: Opens file, may write error messages, sets g_exit_status
 * on error
 */

int	safe_open(const char *file, int flag)
{
	int	fd;

	if (!file)
		return (-1);
	if (flag == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else if (flag == 2)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
	{
		if (errno == ENOENT)
			print_error_exit("no sush file or directory\n", file, 1);
		else if (errno == EACCES)
			print_error_exit("permission denied\n", file, 1);
		else if (errno == EISDIR)
			print_error_exit("is a directory\n", file, 1);
		else
			print_error_exit("cannot open file\n", file, 1);
	}
	return (fd);
}
/*
 * Handles cleanup and error reporting for execve failures.
 * Frees allocated memory for command path, name, and environment array
 * before reporting execution errors. Determines appropriate error message
 * and exit code based on errno (permission denied vs general failure).
 *
 * @param cmd_path: Pointer to command path string to free
 * @param cmd_name: Pointer to command name string to free
 * @param envp: Pointer to environment array to free
 * Side effects: Frees memory, reports error, terminates process
 */

void	execve_error_cleanup(char **cmd_path, char **cmd_name, char **envp)
{
	int	i;

	if (cmd_path)
		safe_free((*cmd_path));
	if (cmd_name)
		safe_free((*cmd_name));
	if (envp)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
		free(envp);
	}
	if (errno == EACCES)
		print_error_exit(*cmd_name, "permession denied", 126);
	else
		print_error_exit(*cmd_name, "command not found", 127);
}
