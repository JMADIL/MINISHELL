/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:47 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 22:05:50 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Frees a null-terminated array of strings (double pointer).
 * Iterates through the array, freeing each individual string before
 * freeing the array pointer itself. Used for cleaning up command
 * argument arrays and other string arrays.
 *
 * @param cmd: Null-terminated array of strings to free
 * Side effects: Frees all allocated memory for strings and array
 */
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

int	exec_builtin(t_cmdarg *shell, t_list **env)
{
	int	result;

	if (ft_strcmp(shell->cmd[0], "cd") == 0)
		result = builtin_cd(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "echo") == 0)
		result = builtin_echo(shell->cmd, shell);
	else if (ft_strcmp(shell->cmd[0], "env") == 0)
		result = builtin_env(env);
	else if (ft_strcmp(shell->cmd[0], "exit") == 0)
		result = builtin_exit(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "export") == 0)
		result = builtin_export(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "pwd") == 0)
		result = builtin_pwd(env);
	else if (ft_strcmp(shell->cmd[0], "unset") == 0)
		result = builtin_unset(shell->cmd, env);
	else
		return (1);
	g_exit_status = result;
	return (result);
}
