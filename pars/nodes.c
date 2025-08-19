/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:24:13 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/19 04:50:32 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell"

t_cmdarg	*ft_init_node(void)
{
	t_cmdarg	*node;

	node = malloc(sizeof(t_cmdarg));
	if (!node)
		return (NULL);
	node->input = NULL;
	node->redirections = NULL;
	node->is_builtin = false;
	node->next = NULL;
	node->output = NULL;
	node->cmd = NULL;
	node->cmdsize = 0;
	node->cmd_capacity = 0;
	return (node);
}

t_cmdarg	*get_next_node(t_token	*token_list)
{
	t_cmdarg	*node;
	int			capa;

	if(!token_list || !token_list->current)
		return (NULL);
	node = ft_init_node();
	capa = ft_toksize(token_list);
	node->cmd = malloc(sizeof(char	*) * (capa + 1));
	if(!node->cmd)
		return (free(node), NULL);
	node->cmd_capacity = capa;
	if(token_list->current->type == PIPE)
		token_list->current = token_list->current->next;
	if (!token_list->current)
    	return (free(node->cmd), free(node), NULL);
	while(token_list->current && token_list->current->type != PIPE)
	{
		if (token_list->current && ft_is_cmd(token_list->current))
        	->>parsing_word(&node, token_list);<<-
		else if (token_list->current && ft_isredi(token_list->current))
        	->>parsing_redi(&node, token_list);<<-
		token_list->current = token_list->current->next;
	}
	node->cmd[node->cmdsize] = NULL;
		return (node);
}
