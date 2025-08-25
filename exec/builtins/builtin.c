/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:47 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 08:54:36 by ajamoun          ###   ########.fr       */
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

int	setup_builtin_io(t_cmdarg *shell, int *input_fd, int *output_fd)
{
	t_redi_list	*input_redir;
	t_redi_list	*output_redir;

	*input_fd = 0;
	*output_fd = 1;
	if (!shell->redirections)
		return (0);
	if (shell->redirections)
	{
		input_redir = shell->redirections;
		if (input_redir->type == INPUT)
		{
			*input_fd = open(input_redir->next->file, O_RDONLY);
			if (*input_fd == -1)
			{
				perror(input_redir->file);
				return (-1);
			}
		}
		else if (input_redir->type == HEREDOC)
		{
			if (input_redir->heredoc_fd != -1)
				*input_fd = input_redir->heredoc_fd;
			else if (input_redir->tmp_fd != -1)
				*input_fd = input_redir->tmp_fd;
		}
	}
	if (shell->redirections->type)
	{
		output_redir = shell->redirections;
		if (output_redir->type == OUTPUT)
		{
			*output_fd = open(output_redir->file, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			if (*output_fd == -1)
			{
				perror(output_redir->file);
				if (*input_fd != STDIN_FILENO)
					close(*input_fd);
				return (-1);
			}
		}
		else if (output_redir->type == APPEND)
		{
			*output_fd = open(output_redir->file, O_WRONLY | O_CREAT | O_APPEND,
					0644);
			if (*output_fd == -1)
			{
				perror(output_redir->file);
				if (*input_fd != STDIN_FILENO)
					close(*input_fd);
				return (-1);
			}
		}
	}
	return (0);
}

void	cleanup_builtin_io(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
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
	printf("Executing builtin: %s\n", shell->cmd[0]);
	printf("Input FD: %d, Output FD: %d\n", input_fd, output_fd);
	if (ft_strcmp(shell->cmd[0], "cd") == 0)
		result = builtin_cd(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "echo") == 0)
		result = builtin_echo(shell->cmd, shell, output_fd);
	else if (ft_strcmp(shell->cmd[0], "env") == 0)
		result = builtin_env(env, output_fd);
	else if (ft_strcmp(shell->cmd[0], "exit") == 0)
		result = builtin_exit(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "export") == 0)
		result = builtin_export(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "pwd") == 0)
		result = builtin_pwd(env, output_fd);
	else if (ft_strcmp(shell->cmd[0], "unset") == 0)
		result = builtin_unset(shell->cmd, env);
	else
	{
		cleanup_builtin_io(input_fd, output_fd);
		return (1);
	}
	cleanup_builtin_io(input_fd, output_fd);
	g_exit_status = result;
	return (result);
}
