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
	int	index;

	index = 0;
	while (input[index] && input[index] != '"' && input[index] != '\'')
		index++;
	return (index);
}

bool	ft_quote_checker(char *input)
{
	int		current_pos;
	char	quote_char;

	current_pos = 0;
	while (input[current_pos])
	{
		current_pos += no_need_chars(input + current_pos);
		if (input[current_pos] == '"' || input[current_pos] == '\'')
		{
			quote_char = input[current_pos];
			current_pos++;
			while (input[current_pos] && input[current_pos] != quote_char)
				current_pos++;
			if (!input[current_pos])
				return (printf("UNCLOSED QUOTES\n"), g_exit_status = 258,
					false);
			if (input[current_pos])
				current_pos++;
		}
		else if (input[current_pos])
			current_pos++;
	}
	return (true);
}
