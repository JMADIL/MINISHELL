/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:26:22 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 13:31:06 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_words_with_braces(const char *s, char sep)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == sep)
			s++;
		if (!*s)
			break ;
		count++;
		while (*s && *s != sep)
		{
			if (*s == '$' && *(s + 1) == '{')
			{
				s += 2;
				while (*s && *s != '}')
					s++;
				if (*s == '}')
					s++;
			}
			else
				s++;
		}
	}
	return (count);
}

static void	get_next_word_with_braces(char **s, size_t *len, char sep,
		char **start)
{
	char	*str;

	str = *s;
	while (*str && *str == sep)
		str++;
	*start = str;
	while (*str && *str != sep)
	{
		if (*str == '$' && *(str + 1) == '{')
		{
			str += 2;
			while (*str && *str != '}')
				str++;
			if (*str == '}')
				str++;
		}
		else
			str++;
	}
	*len = str - *start;
	*s = str;
}

static void	copy_token_to_buffer(size_t len, size_t j, char *start, char **buff)
{
	buff[j] = malloc(len + 1);
	if (!buff[j])
		return ;
	ft_strlcpy(buff[j], start, len + 1);
}

char	**split_with_braces(const char *s, char sep)
{
	char	**buff;
	char	*tmp;
	char	*start;
	size_t	len;
	size_t	j;

	if (!s)
		return (NULL);
	buff = malloc(sizeof(char *) * (count_words_with_braces(s, sep) + 1));
	if (!buff)
		return (NULL);
	tmp = (char *)s;
	j = 0;
	while (j < (size_t)count_words_with_braces(s, sep))
	{
		get_next_word_with_braces(&tmp, &len, sep, &start);
		copy_token_to_buffer(len, j, start, buff);
		j++;
	}
	buff[j] = NULL;
	return (buff);
}
