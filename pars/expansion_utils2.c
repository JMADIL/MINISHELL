/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 01:56:36 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/23 00:37:39 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_condition(t_token **token, ssize_t dollar_pos)
{
	return ((*token)->value[dollar_pos + 1] == '$' || (*token)->value[dollar_pos
		+ 1] == '?' || ft_isdigit((*token)->value[dollar_pos + 1]));
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_expand(t_token **token, t_list	*minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr((*token)->value, dollar_pos + 1,
            ft_get_varlen((*token)->value + dollar_pos + 1));
	expanded = ft_getvar(var, minienv);
	free(var);
	return(expanded); 
}

char	*ft_getenv(char *var, t_list *minienv)
{
	t_list	*tmp;
	
	tmp = minienv;
	if(tmp == NULL)
		return (NULL);
	while(tmp)
	{
		if (ft_strcmp(var, tmp->key) == 0)
    		return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*ft_getvar(char *var, t_list *minienv)
{
	char	*value;
	value = ft_getenv(var, minienv);
	if(!value)
	{
		value = ft_strdup("");
		return (value);
	}
	return (value);
}
