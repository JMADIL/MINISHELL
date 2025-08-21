/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 04:24:13 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/21 06:25:27 by ajamoun          ###   ########.fr       */
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
        	parsing_word(&node, token_list);
		else if (token_list->current && ft_isredi(token_list->current))
        	parsing_redi(&node, token_list);
		token_list->current = token_list->current->next;
	}
	node->cmd[node->cmdsize] = NULL;
		return (node);
}

t_cmdarg	*ft_newnode(t_cmdarg *node)
{
	t_cmdarg	*new;
	int			i;

	new = malloc(sizeof(t_cmdarg));
	if (!new)
		return (NULL);
	new->cmdsize = node->cmdsize;
	new->cmd_capacity = node->cmd_capacity;
	new->cmd = malloc(sizeof(char *) * (node->cmd_capacity + 1));
	if (!new->cmd)
		return (free(new), NULL);
	i = -1;
	while (++i < node->cmdsize)
	{
		if (node->cmd[i])
			new->cmd[i] = ft_strdup(node->cmd[i]);
		else
			new->cmd[i] = NULL;
	}
	new->cmd[i] = NULL;
	new->is_builtin = node->is_builtin;
	new->redirections = node->redirections;
	new->input = node->input;
	new->next = NULL;
	return (new);
}

void	ft_nodeadd_back(t_cmdarg **lst, t_cmdarg *new)
{
	t_cmdarg	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

bool	resize_cmd_array(t_cmdarg **node, int new_capacity)
{
	char	**new_cmd;
	int		i;

	if (new_capacity <= (*node)->cmd_capacity)
		return (true);
	new_cmd = malloc(sizeof(char *) * (new_capacity + 1));
	if (!new_cmd)
		return (false);
	i = 0;
	while (i < (*node)->cmdsize)
	{
		new_cmd[i] = (*node)->cmd[i];
		i++;
	}
	while (i <= new_capacity)
	{
		new_cmd[i] = NULL;
		i++;
	}
	free((*node)->cmd);
	(*node)->cmd = new_cmd;
	(*node)->cmd_capacity = new_capacity;
	return (true);
}
