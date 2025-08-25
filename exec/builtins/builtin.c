/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:47 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 12:02:43 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_arr(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "unset") == 0);
}

void	cleanup_builtin_io(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
}

static int	run_builtin_cmd(t_cmdarg *shell, t_list **env, int output_fd)
{
	if (ft_strcmp(shell->cmd[0], "cd") == 0)
		return (builtin_cd(shell->cmd, env));
	else if (ft_strcmp(shell->cmd[0], "echo") == 0)
		return (builtin_echo(shell->cmd, shell, output_fd));
	else if (ft_strcmp(shell->cmd[0], "env") == 0)
		return (builtin_env(env, output_fd));
	else if (ft_strcmp(shell->cmd[0], "exit") == 0)
		return (builtin_exit(shell->cmd, env));
	else if (ft_strcmp(shell->cmd[0], "export") == 0)
		return (builtin_export(shell->cmd, env));
	else if (ft_strcmp(shell->cmd[0], "pwd") == 0)
		return (builtin_pwd(env, output_fd));
	else if (ft_strcmp(shell->cmd[0], "unset") == 0)
		return (builtin_unset(shell->cmd, env));
	return (-1);
}

int	exec_builtin(t_cmdarg *shell, t_list **env)
{
	int	result;
	int	input_fd;
	int	output_fd;

	if (setup_builtin_io(shell, &input_fd, &output_fd) == -1)
	{
		g_exit_status = 1;
		return (1);
	}
	result = run_builtin_cmd(shell, env, output_fd);
	if (result == -1)
	{
		cleanup_builtin_io(input_fd, output_fd);
		return (1);
	}
	cleanup_builtin_io(input_fd, output_fd);
	g_exit_status = result;
	return (result);
}
