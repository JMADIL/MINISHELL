/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 03:36:35 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/25 11:34:52 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_norm(t_token **token, t_list *minienv, ssize_t dpos)
{
	char	*expanded;
	char	*tmp;
	char	*tmp2;

	expanded = ft_expand(token, minienv, dpos);
	tmp = ft_strjoin_free(ft_substr((*token)->value, 0, dpos), expanded);
	tmp2 = help_tmp2(token, dpos);
	free((*token)->value);
	(*token)->value = ft_strjoin_free(tmp, tmp2);
	free(tmp2);
	(*token)->variable = true;
	free(expanded);
}

void	expand_exit_status(t_token **token)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*tmp;
	char	*tmp2;
	char	*leak;

	dollar_pos = dollar_position((*token)->value);
	if ((*token)->value[dollar_pos + 1] == '?')
		expanded = ft_itoa(g_exit_status);
	else if ((*token)->value[dollar_pos + 1] == '$')
		expanded = ft_itoa(getpid());
	else
		expanded = ft_strdup("");
	leak = (*token)->value;
	tmp = ft_strjoin_free(ft_substr((*token)->value, 0, dollar_pos), expanded);
	tmp2 = ft_substr((*token)->value, dollar_pos + 2,
			ft_strlen((*token)->value));
	(*token)->value = ft_strjoin_free(tmp, tmp2);
	(*token)->variable = true;
	free(expanded);
	free(leak);
	free(tmp2);
}

void	expand_variables(t_token **token, t_list *minienv)
{
	ssize_t	dpos;

	if ((*token)->type == SINGLE_QUOTE)
		return ;
	dpos = dollar_position((*token)->value);
	while (dpos != -1)
	{
		if (ft_condition(token, dpos))
		{
			expand_exit_status(token);
			dpos = dollar_position((*token)->value);
			continue ;
		}
		expand_norm(token, minienv, dpos);
		dpos = dollar_position((*token)->value);
	}
}
