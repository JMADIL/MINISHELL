/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:49:59 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/19 05:23:06 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell"

void	*parsing_word(t_cmdarg	**node, t_token *token_list)
{
	char	*value;
	
	value = ft_strdup(token_list->current->value);
	if(!value)
		return(perror("malloc failure"), g_exit_status = 1, NULL);
	if(token_list->current->variable == true && token_list->current->type == WORD)
	{
		->>handle_split_word(node, token_list, value);<<-
	}
	else
	{
		//Ensures the cmd array has enough space (reallocates if needed)
		if(!->>ft_extend_cmd_for_word(node)<<-)
			return (free(value), NULL);
		->>ft_add_or_join_word(node, token_list, val);<<-
	}
	return (NULL);
}

void	*parsing_redi(t_cmdarg **node, t_token *token_list)
{
	
}
