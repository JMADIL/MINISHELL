/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_expansion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 03:36:35 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/16 04:07:03 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell"

void	expand_variables(t_token **token, t_list *minienv)
{
	ssize_t dpos;
	
	if ((*token)->type == SINGLE_QUOTE)
    	return ;
	dpos = ->>dollar_position((*token)->value)<<-;
	//Process All Variables
	while(dpos != -1)
	{
		//Determine Variable Type
		if(ft_condition(token, dpos))
		{
			->>ft_expand_exit_status(token);<<-
    		dpos = ->>dollar_position((*token)->value)<<-;
    		continue ;
		}
		//Handle Regular Variables
		->>expand_norm(token, minienv, dpos);<<-
		dpos = ->>dollar_position((*token)->value)<<-;
	}
}
