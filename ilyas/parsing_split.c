#include "../minishell.h"

/* ============================================================================
 * 1) Count tokens, respecting ${VAR} braces as single words

	* ============================================================================ */
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

/* ============================================================================
 * 2) Get next word start & length, respecting ${VAR} braces

	* ============================================================================ */
static void	get_next_word_with_braces(char **s, size_t *len, char sep,
		char **start)
{
	char	*str;
	char	*end;

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

/* ============================================================================
 * 3) Copy a token into destination buffer at index j

	* ============================================================================ */
static void	copy_token_to_buffer(size_t len, size_t j, char *start, char **buff)
{
	buff[j] = malloc(len + 1);
	if (!buff[j])
		return ;
	ft_strlcpy(buff[j], start, len + 1);
}

/* ============================================================================
 * 4) Split string into tokens based on sep, handling ${VAR} braces

	* ============================================================================ */
char	**split_with_braces(const char *s, char sep)
{
	char	**buff;
	char	*start;
	char	*tmp;
	size_t	len;
	size_t	j;
	int		words;

	if (!s)
		return (NULL);
	words = count_words_with_braces(s, sep);
	buff = malloc(sizeof(char *) * (words + 1));
	if (!buff)
		return (NULL);
	tmp = (char *)s;
	j = 0;
	while (j < (size_t)words)
	{
		get_next_word_with_braces(&tmp, &len, sep, &start);
		copy_token_to_buffer(len, j, start, buff);
		j++;
	}
	buff[j] = NULL;
	return (buff);
}
