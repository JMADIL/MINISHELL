
#include "../minishell.h"

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

void	exec_external_command(t_cmdarg *current_cmd, t_list *env)
{
	char	*cmd_path;
	char	**envp;
	char	*cmd_name;
	int		no_file;

	envp = NULL;
	cmd_name = NULL;
	no_file = 0;
	if (current_cmd == NULL || current_cmd->cmd[0] == NULL)
		exit(0);
	if (current_cmd->cmd[0])
		cmd_name = ft_strdup(current_cmd->cmd[0]);
	cmd_path = check_exec(current_cmd->cmd[0], env, &no_file);
	if (cmd_path == NULL || !cmd_name[0])
	{
		free(cmd_name);
		cmd_not_found_exit(current_cmd, no_file);
	}
	if (is_directory(cmd_path))
		ft_free_isdir(&cmd_path, &cmd_name, current_cmd);
	envp = get_env(env);
	if (envp == NULL)
		exec_malloc_fail(cmd_path, cmd_name);
	if (execve(cmd_path, current_cmd->cmd, envp) == -1)
		execve_error_cleanup(&cmd_path, &cmd_name, envp);
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

void	exec_builtin_in_child(t_cmdarg *current_cmd, t_list **env)
{
	char	**cmd;
	cmd = NULL;

	if (!current_cmd || !current_cmd->cmd || !current_cmd->cmd[0])
		return ;
	if (cmd && cmd[0] && is_builtin(current_cmd->cmd[0]))
	{
		if (exec_builtin(current_cmd, env))
		{
			exit(g_exit_status);
		}
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

void	exec_child_process(t_cmdarg *current_cmd, t_list *env, int tmp_in,
		int p_fd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (tmp_in != STDIN_FILENO && dup2(tmp_in, STDIN_FILENO) == -1)
		print_error_exit("dup2", "failed to dup input", 1);
	if (tmp_in != STDIN_FILENO)
		close(tmp_in);
	if (current_cmd->next != NULL)
	{
		if (dup2(p_fd[1], STDOUT_FILENO) == -1)
			print_error_exit("dup2", "failed to dup output", 1);
		close(p_fd[1]);
		close(p_fd[0]);
	}
	process_input_redirections(current_cmd->redirections);
	process_output_redirections(current_cmd->redirections);

	// if (is_builtin(current_cmd->cmd[0])){
	// 	exec_builtin_in_child(current_cmd, &env);
	// }
	// else {
		exec_external_command(current_cmd, env);
	// }
	_exit(127);
}
