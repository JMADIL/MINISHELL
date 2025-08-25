/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:11 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 08:45:40 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_n_flag(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

char	*join_args_from_index(char **cmd, int i)
{
	char	*result;
	char	*temp;
	char	*with_space;

	if (!cmd || !cmd[i])
		return (ft_strdup(""));
	result = ft_strdup(cmd[i]);
	if (!result)
		return (NULL);
	i++;
	while (cmd[i])
	{
		with_space = ft_strjoin(result, " ");
		if (!with_space)
			return (free(result), NULL);
		temp = ft_strjoin(with_space, cmd[i]);
		free(result);
		free(with_space);
		if (!temp)
			return (NULL);
		result = temp;
		i++;
	}
	return (result);
}

void	print_echo_output(char *tmp, int n_flag, int out)
{
	if (tmp)
		write(out, tmp, ft_strlen((char *)tmp));
	if (!n_flag)
		write(out, "\n", 1);
}

int	builtin_echo(char **cmd, t_cmdarg *shell, int out)
{
	int		i;
	int		n_flag;
	char	*joined;

	(void)shell;
	i = 1;
	n_flag = 0;
	while (cmd[i] && is_valid_n_flag(cmd[i]))
	{
		n_flag = 1;
		i++;
	}
	joined = join_args_from_index(cmd, i);
	if (!joined)
		return (1);
	print_echo_output(joined, n_flag, out);
	free(joined);
	return (0);
}
