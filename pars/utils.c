/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:59:55 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/25 11:46:20 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

int	ft_isspecial(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	op_is_duplicated(t_lexer *lexer, char op[3])
{
	return ((op[0] == '>' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '>') || (op[0] == '<'
			&& lexer->pos + 1 < lexer->len && lexer->input[lexer->pos
				+ 1] == '<'));
}

ssize_t	dollar_position(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0' && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '$' || str[i + 1] == '?'))
			return (i);
		i++;
	}
	return (-1);
}

bool	ft_is_cmd(t_token *current)
{
	return (current->type == WORD || current->type == DOUBLE_QUOTE
		|| current->type == SINGLE_QUOTE);
}
