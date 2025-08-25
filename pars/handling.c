/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:09:27 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/25 11:39:02 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_init_fields(t_token *token)
{
	token->variable = false;
	token->current = NULL;
	token->next = NULL;
	token->prev = NULL;
}

void	ft_set_token_type(t_token *token, int op_len, char *op, bool *heredoc)
{
	if (op_len == 2)
	{
		if (op[0] == '>')
			token->type = APPEND;
		else
		{
			token->type = HEREDOC;
			*heredoc = true;
		}
	}
	else if (op[0] == '|')
		token->type = PIPE;
	else
	{
		if (op[0] == '>')
			token->type = OUTPUT;
		else
			token->type = INPUT;
	}
}

t_token	*handling_operator(t_lexer *lexer, bool *heredoc)
{
	char	op[3];
	int		op_len;
	t_token	*token;

	op[0] = lexer->input[lexer->pos];
	op[1] = '\0';
	op_len = 1;
	if (op_is_duplicated(lexer, op))
	{
		op[1] = lexer->input[lexer->pos + 1];
		op[2] = '\0';
		op_len = 2;
	}
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(op);
	if (!token->value)
		return (free(token), NULL);
	token->addspace = false;
	ft_init_fields(token);
	ft_set_token_type(token, op_len, op, heredoc);
	lexer->pos += op_len;
	return (token);
}

t_token	*handling_quotes(t_lexer *lexer, char quote)
{
	int		start;
	char	*content;
	t_token	*token;

	start = ++lexer->pos;
	while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote)
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (printf(QUOTES), g_exit_status = 258, NULL);
	content = ft_substr(lexer->input, start, lexer->pos - start);
	lexer->pos++;
	token = malloc(sizeof(t_token));
	if (!token || !content)
		return (NULL);
	if (quote == '\'')
		token->type = SINGLE_QUOTE;
	else
		token->type = DOUBLE_QUOTE;
	token->value = content;
	if (ft_isspace(lexer->input[lexer->pos]))
		token->addspace = true;
	else
		token->addspace = false;
	ft_init_fields(token);
	return (token);
}

t_token	*handling_words(t_lexer *lexer)
{
	t_token	*token;
	char	*value;
	int		start;

	start = lexer->pos;
	while (lexer->pos < lexer->len && !ft_isspace(lexer->input[lexer->pos])
		&& !ft_isspecial(lexer->input[lexer->pos])
		&& lexer->input[lexer->pos] != '\'' && lexer->input[lexer->pos] != '"')
		lexer->pos++;
	value = ft_substr(lexer->input, start, lexer->pos - start);
	token = malloc(sizeof(t_token));
	if (!value || !token)
		return (NULL);
	token->type = WORD;
	token->value = value;
	if (ft_isspace(lexer->input[lexer->pos]))
		token->addspace = true;
	else
		token->addspace = false;
	ft_init_fields(token);
	return (token);
}
