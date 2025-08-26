/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:19:40 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 14:20:49 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	exec_builtin_in_child(t_cmdarg *current_cmd, t_list **env)
{
	if (!current_cmd || !current_cmd->cmd || !current_cmd->cmd[0])
		return ;
	if (current_cmd->cmd && current_cmd->cmd[0] && is_builtin(current_cmd->cmd[0]))
	{
		if (exec_builtin(current_cmd, env))
		{
			exit(g_exit_status);
		}
	}
}

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
	exec_external_command(current_cmd, env);
	_exit(127);
}
