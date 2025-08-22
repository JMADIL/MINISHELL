/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:08:16 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/22 05:47:41 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*newtok(t_token *token)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = token->type;
	new_token->value = ft_strdup(token->value);
	new_token->addspace = token->addspace;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->variable = false;
	return (new_token);
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

bool	ft_tojoin(t_lexer *lexer)
{
	return (lexer->input[lexer->pos] != ' ' && lexer->input[lexer->pos] != '\0'
		&& !ft_isspecial(lexer->input[lexer->pos]));
}

t_token *ft_jointok(t_token *token, t_lexer **lexer, t_list *minienv, bool *heredoc)
{
	t_token *new_token;
	char *value;
	//Expand Variables in Current Token
	if(*heredoc == false)
		expand_variables(&token, minienv);
	//Get Next Token
	new_token = get_next_token(*lexer, minienv, heredoc);
	if (!new_token && g_exit_status == 258)
    	return (ft_free_token(token), NULL);
	//Expand Variables in Next Token
	if(*heredoc == false)
		expand_variables(&token, minienv);
	//Special Dollar-Quote Joining
	if(ft_strlen(token->value) > 0
		&& token->value[ft_strlen(token->value) - 1] == '$'
		&& (new_token->type == DOUBLE_QUOTE || new_token->type == SINGLE_QUOTE))
	{
		return (ft_dollar_joining(token, new_token));		
	}
	//Preserve Token Type
	if (token->type != WORD)
    	new_token->type = token->type;
	//Join Values
	value = ft_strjoin(token->value, new_token->value);
	//Memory Management and Cleanup
	ft_free_token(token);           // Free original token completely
	free(new_token->value);         // Free old value of new_token
	new_token->value = value;       // Assign joined string
	return (new_token);             // Return modified new_token
}

t_token	*get_next_token(t_lexer *lexer, t_list *minienv, bool heredoc)
{
	char	current_char;
	t_token	*token;

	// Skip Whitespace
	while (lexer->pos < lexer->len && ft_isspace(lexer->input[lexer->pos]))
		lexer->pos++;
	// If we've reached the end of the input string, return NULL
	if (lexer->pos >= lexer->len)
		return (NULL);
	// Store the character we're about to process
	current_char = lexer->input[lexer->pos];
	// Handle Special Characters (Operators)
	if (ft_isspecial(current_char))
		return (handling_operator(lexer, heredoc));
	// Handle Non-Special Characters
	else
	{
		if (current_char == '\'' || current_char == '"')
		{
			token = handling_quotes(lexer, current_char);
			if (!token)
				return (NULL);
		}
		else
			token = handling_words(lexer);
		// Checks if the next character should be joined to current token
		if (ft_tojoin(lexer))
			// Combines adjacent tokens that should be treated as one
			token = ft_jointok(token, &lexer, minienv, heredoc);
		return (token);
	}
}
