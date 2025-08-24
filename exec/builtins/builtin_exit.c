/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:17 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:07:55 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '+' || str[0] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_exit_error(char **cmd, t_list **env)
{
	fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cmd[1]);
	free_env_list(env);
	exit(255);
}

int	builtin_exit(char **cmd, t_list **env)
{
	long	exit_code;

	write(1, "exit\n", 5);
	if (cmd[1] && cmd[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	if (cmd[1])
	{
		if (!numeric(cmd[1]))
			handle_exit_error(cmd, env);
		exit_code = ft_atoi(cmd[1]);
		exit_code = exit_code % 256;
		if (exit_code < 0)
			exit_code += 256;
		g_exit_status = (int)exit_code;
	}
	free_env_list(env);
	exit(g_exit_status);
}
