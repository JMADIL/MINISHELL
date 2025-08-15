/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:59:55 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/15 10:39:05 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell"

int ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

int	ft_isspecial(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	op_is_duplicated(t_lexer lexer, char op[3])
{
	return ((op[0] == '>' && lexer->pos + 1 < lexer->len
		&& lexer->input[lexer->pos + 1] == '>') || (op[0] == '<'
		&& lexer->pos + 1 < lexer->len && lexer->input[lexer->pos
			+ 1] == '<'));
}
