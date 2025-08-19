#include "help.h"

/*
 * Checks if a given path points to a directory.
 * Uses stat system call to determine if the path exists and represents
 * a directory. Only checks paths that contain forward slashes and are
 * not empty or null.
 *
 * @param path: File path string to check
 * @return: true if path is a directory, false otherwise
 */

bool	is_directory(const char *path)
{
	struct stat	st;

	if (!path || *path == '\0')
		return (false);
	if (!ft_strchr(path, '/'))
		return (false);
	if (stat(path, &st) == -1)
		return (false);
	return (S_ISDIR(st.st_mode));
}


/*
 * Handles the execution of external commands.
 * Manages the complete process of executing non-builtin commands including
 * path resolution, directory checking, environment preparation, and final
 * execution via execve. Handles various error conditions and cleanup.
 *
 * @param current_cmd: Command structure containing command and arguments
 * @param env: Environment variables list
 * Side effects: May exit process, allocates/frees memory, executes command
 */

static void	exec_external_command(t_cmdarg *current_cmd, t_shell *shell)
{
	char	*cmd_path;

	if (!current_cmd || !current_cmd->cmd || !current_cmd->cmd[0])
		print_error_exit(NULL, "command not found", 127);

	if (is_directory(current_cmd->cmd[0]))
		print_error_exit(current_cmd->cmd[0], "is a directory", 126);

	cmd_path = validate_exec_path(current_cmd->cmd[0]);
	if (!cmd_path)
		cmd_path = search_path_env(current_cmd->cmd[0], shell); 

	if (!cmd_path)
		print_error_exit(current_cmd->cmd[0], "command not found", 127);

	if (execve(cmd_path, current_cmd->cmd, shell) == -1)
	{
		free(cmd_path);
		if (errno == EACCES)
			print_error_exit(current_cmd->cmd[0], "permission denied", 126);
		else
			print_error_exit(current_cmd->cmd[0], "command not found", 127);
	}
}

/*
 * Executes builtin commands in child processes.
 * Checks if the current command is a builtin and executes it if so.
 * Handles the exit status and terminates the child process after
 * builtin execution with the appropriate exit code.
 *
 * @param current_cmd: Command structure to check and execute
 * @param env: Pointer to environment variables list
 * Side effects: May exit process with global exit status
 */

static void	exec_builtin_in_child(t_cmdarg *current_cmd, t_shell *shell)
{
	int	exit_code;

	if (!current_cmd || !current_cmd->cmd || !current_cmd->cmd[0])
		return ;
	if (is_builtin(current_cmd->cmd[0])) 
	{
		exit_code = execute_builtin(current_cmd, shell);
		_exit(exit_code);
	}
}

/*
 * Main function for child process execution in pipeline.
 * Sets up the child process environment including signal handling,
 * file descriptor duplication for pipes, redirection handling,
 * and final command execution (either builtin or external).
 *
 * @param current_cmd: Command structure to execute
 * @param env: Environment variables list
 * @param tmp_in: Input file descriptor from previous command in pipeline
 * @param p_fd: Pipe file descriptors for output to next command
 * Side effects: Modifies file descriptors, handles redirections,
 * executes command
 */

void	exec_child_process(t_cmdarg *current_cmd, t_shell *shell,
		int tmp_in, int p_fd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	if (dup2(tmp_in, STDIN_FILENO) == -1)
		print_error_exit("dup2", "failed to dup input", 1);
	close(tmp_in);

	if (current_cmd->next != NULL)
	{
		if (dup2(p_fd[1], STDOUT_FILENO) == -1)
			print_error_exit("dup2", "failed to dup output", 1);
	}
	close(p_fd[0]);
	close(p_fd[1]);

	if (!process_input_redirections(current_cmd->input))
		_exit(1);
	if (!process_output_redirections(current_cmd->output))
		_exit(1);

	if (is_builtin(current_cmd->cmd[0]))
		exec_builtin_in_child(current_cmd, shell);
	else
		exec_external_command(current_cmd, shell);

	_exit(127);
}
