/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:39:22 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/22 05:46:54 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_or_add_first_word(t_cmdarg **node, char **split_words,
		bool first_word_joins)
{
	char	*tmp;

	if (first_word_joins && (*node)->cmdsize > 0)
	{
		tmp = (*node)->cmd[(*node)->cmdsize - 1];
		(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(tmp, split_words[0]);
		free(tmp);
	}
	else
		(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[0]);
}

void	extend_cmd_array_if_needed(t_cmdarg **node, int needed_capacity)
{
	if (needed_capacity > (*node)->cmd_capacity)
	{
		if (!resize_cmd_array(node, needed_capacity * 2))
		{
			perror("Failed to resize cmd array");
			g_exit_status = 1;
		}
	}
}

void	add_split_words(t_cmdarg **node, char **split_words,
		bool first_word_joins)
{
	int	i;
	int	words_count;
	int	needed_capacity;

	if (!split_words || !split_words[0])
		return ;
	words_count = 0;
	while (split_words[words_count])
		words_count++;
	needed_capacity = (*node)->cmdsize + words_count;
	if (first_word_joins && (*node)->cmdsize > 0)
		needed_capacity--;
	extend_cmd_array_if_needed(node, needed_capacity);
	join_or_add_first_word(node, split_words, first_word_joins);
	i = 1;
	while (split_words[i])
	{
		(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[i]);
		i++;
	}
}

void	ft_free_split_words(char **split_words)
{
	int	i;

	if (!split_words)
		return ;
	i = 0;
	while (split_words[i])
		free(split_words[i++]);
	free(split_words);
}

void	handle_split_word(t_cmdarg **node, t_token *token_list, char *value)
{
	char	**split_words;
	bool	should_join;

	if (!value || !*value)
	{
		free(value);
		return ;
	}
	split_words = ft_split(value, " \t");
	if (!split_words)
	{
		free(value);
		perror("malloc failure in ft_split");
		g_exit_status = 1;
		return ;
	}
	if (!split_words[0])
		return (ft_free_split_words(split_words), free(value));
	should_join = (token_list->current->prev != NULL
			&& token_list->current->prev->addspace == false);
	add_split_words(node, split_words, should_join);
	ft_free_split_words(split_words);
	free(value);
}
