/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:09:46 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 13:16:40 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	create_pipe_safe(int pip[2])
{
	if (pipe(pip) == -1)
	{
		perror("minishell: pipe");
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

void	handle_parent_fd(int *tmp_in, int pip_fd[2], t_cmdarg *current_cmd)
{
	if (*tmp_in != STDIN_FILENO)
		close(*tmp_in);
	*tmp_in = pip_fd[0];
	if (current_cmd->next)
		close(pip_fd[1]);
	if (current_cmd->redirections)
		ft_close_pipe(current_cmd->redirections);
}

void	wait_for_all_children(int *status, pid_t last_pid)
{
	pid_t	pid;
	int		last_status;

	last_status = 0;
	pid = wait(status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(*status))
				last_status = WEXITSTATUS(*status);
			else if (WIFSIGNALED(*status))
			{
				if (WTERMSIG(*status) == SIGPIPE)
					last_status = 0;
				else
					last_status = 128 + WTERMSIG(*status);
			}
		}
		pid = wait(status);
	}
	g_exit_status = last_status;
}

int	ft_fork_and_exec(t_cmdarg *cmd, t_list *env, int *tmp_in,
		pid_t *last_cmd_pid)
{
	int		pip_fd[2];
	pid_t	pid;

	if (cmd->next && !create_pipe_safe(pip_fd))
		return (0);
	pid = fork();
	if (pid == -1)
		return (g_exit_status = 1, perror("minishell: fork"), 0);
	if (pid == 0)
		exec_child_process(cmd, env, *tmp_in, pip_fd);
	else
	{
		*last_cmd_pid = pid;
		handle_parent_fd(tmp_in, pip_fd, cmd);
	}
	return (1);
}

int	execution(t_cmdarg *current_cmd, t_list *env)
{
	int		tmp_in;
	pid_t	last_cmd_pid;

	tmp_in = 0;
	while (current_cmd)
	{
		if (!ft_fork_and_exec(current_cmd, env, &tmp_in, &last_cmd_pid))
			return (0);
		current_cmd = current_cmd->next;
	}
	finish_exec(last_cmd_pid);
	return (1);
}
