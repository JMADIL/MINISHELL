/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 03:09:27 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/15 10:50:23 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell"

void	ft_init_fields(t_token	*token)
{
	token->variable = false; 
	token->current = NULL;
	token->next = NULL;
	token->prev = NULL;
}

void	ft_set_token_type(t_token token, int op_len, char *op, bool *heredoc)
{
	if(op_len == 2)
	{
		if(op[0] == '>')
			token->type = APPEND;
		else
		{
			token->type = HEREDOC;
			*heredoc = true;
		}
	}
	else if(op[0] == '|')
		token->type = PIPE;
	else
	{
		if(op[0] == '>')
			token->type = OUTPUT;
		else
			token->type = INPUT;	
	}
}

t_token	*handling_operator(t_lexer lexer, bool heredoc)
{
	char op[3];
	int op_len;
	t_token *token;

	op[0] = lexer->input[lexer->pos];
	op[1] = '\0';
	op_len = 1;
	if(op_is_duplicated(lexer, op))
	{
		op[1] = lexer->input[lexer->pos + 1];
		op[2] = '\0';
		op_len = 2;
	}
	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->value = ft_strdup(op);
	if(!token->value)
		return (free(token), NULL);
	token->addspace = false;
	ft_init_token_fields(token);
	//This function likely sets token->type based on the operator
	->>ft_set_token_type(token, op_len, op, heredoc);<<-
	//Advance Lexer Position
	lexer->pos += op_len;
	return (token);
}
