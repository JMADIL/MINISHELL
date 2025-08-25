/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 22:55:21 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/25 11:43:00 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_need_chars(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '"' && input[i] != '\'')
		i++;
	return (i);
}

bool	ft_quote_checker(char *input)
{
	int		i;
	char	quo;

	i = 0;
	while (input[i])
	{
		i += no_need_chars(input + i);
		if (input[i] == '"' || input[i] == '\'')
		{
			quo = input[i];
			i++;
			while (input[i] && input[i] != quo)
				i++;
			if (!input[i])
				return (printf("UNCLOSED QUOTES\n"), g_exit_status = 258,
					false);
			if (input[i])
				i++;
		}
		else if (input[i])
			i++;
	}
	return (true);
}
