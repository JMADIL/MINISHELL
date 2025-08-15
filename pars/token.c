/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:08:16 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/15 10:58:33 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell"

t_token	*newtok(t_token *token)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = token->type new_token->value = ft_strdup(token->value);
	new_token->addspace = token->addspace;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->variable = false;
	return (new_token);
}

t_token	*get_next_token(t_lexer *lexer, t_list *minienv, bool heredoc)
{
	char	current_char;
	t_token	*token;
	
	//Skip Whitespace
	while (lexer->pos < lexer->len && ft_isspace(lexer->input[lexer->pos]))
		lexer->pos++;
	//If we've reached the end of the input string, return NULL
	if (lexer->pos >= lexer->len)
		return (NULL);
	//Store the character we're about to process
	current_char = lexer->input[lexer->pos];
	//Handle Special Characters (Operators)
	if (ft_isspecial(current_char))
		return (handling_operator(lexer, heredoc));
	//Handle Non-Special Characters
	else
	{
		if (current_char == '\'' || current_char == '"')
		{
			token = ->>ft_handle_quotes(lexer, current_char);<<-
			if (!token)
				return (NULL);
		}
		else
			token = ->>ft_handle_word(lexer);<<-
		//Checks if the next character should be joined to current token
		if (->>ft_tojoin(lexer)<<-)
		//Combines adjacent tokens that should be treated as one
			token = ->>ft_jointok(token, &lexer, minienv, heredoc);<<-
		return (token);
	}
}

void	ft_tokadd_back(t_token **token_list, t_token *token)
{
	t_token	*tmp;

	if (!token_list || !token)
		return ;
	if (*token_list == NULL)
	{
		*token_list = token;
		return ;
	}
	tmp = *token_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	token->next = NULL;
}
