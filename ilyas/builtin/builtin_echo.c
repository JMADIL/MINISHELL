/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:11 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 22:05:11 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Validates that a string contains only 'n' characters after "-".
 * Used to verify that echo -n flags contain only 'n' characters,
 * ensuring proper parsing of the no-newline option.
 *
 * @param str: String to validate (should start with "-n")
 * @return: 1 if string contains only 'n' after "-", 0 otherwise
 */
int	is_vaid_n_flag(const char *str)
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

/*
 * Joins command arguments into a single string with spaces.
 *
 * @param cmd: Array of command arguments
 * @param i: Starting index for joining arguments
 * @return: Joined string containing all arguments separated by spaces
 */

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

/*
 * Outputs the echo string with or without a trailing newline.
 *
 * @param tmp: String to output
 * @param n_flag: If non-zero, suppresses trailing newline
 */

void	prent_echo_output(char *tmp, int n_flag)
{
	if (tmp)
		write(STDOUT_FILENO, tmp, ft_strlen((char *)tmp));
	if (!n_flag)
		write(STDOUT_FILENO, "\n", 1);
}
/*
 * Implements the echo builtin command.
 * Outputs the specified arguments to stdout, supporting the -n option.
 *
 * @param cmd: Array of command arguments (cmd[0] is "echo")
 * @param shell: (unused)
 * @return: Always returns 0
 */

int	builtin_echo(char **cmd, t_cmdarg *shell)
{
	int		i;
	int		n_flag;
	char	*joined;

	(void)shell;
	i = 1;
	n_flag = 0;
	while (cmd[i] && is_vaid_n_flag(cmd[i]))
	{
		n_flag = 1;
		i++;
	}
	joined = join_args_from_index(cmd, i);
	if (!joined)
		return (1);
	prent_echo_output(joined, n_flag);
	free(joined);
	return (0);
}
