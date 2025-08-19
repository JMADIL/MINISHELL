
#include "get_next_line.h"


char	*my_strdup(const char *s1)
{
	char	*buff;
	size_t	size;
	int		i;

	i = 0;
	if (!s1)
		return (NULL);
	size = my_strlen(s1);
	buff = malloc(size + 1);
	if (!buff)
		return (NULL);
	while (s1[i])
	{
		buff[i] = s1[i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}

/*
 * Joins two strings into a single concatenated string.
 * Allocates memory for a new string containing the concatenation
 * of s1 and s2. Automatically frees the first string (s1) after
 * joining, which is useful for accumulating buffer content.
 *
 * @param s1: First string (freed after joining)
 * @param s2: Second string to append
 * @return: New concatenated string, NULL on failure
 * Side effects: Frees s1, allocates memory for result
 */
char	*my_strjoin(char *s1, char *s2)
{
	char	*new_str;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1)
		return (my_strdup(s2));
	if (!s2)
		return (my_strdup(s1));
	total_len = my_strlen(s1) + my_strlen(s2) + 1;
	new_str = (char *)malloc(total_len * sizeof(char));
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	i = -1;
	while (s1[++i])
		new_str[i] = s1[i];
	j = -1;
	while (s2[++j])
		new_str[i++] = s2[j];
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}

/*
 * Searches for a character in a string.
 * Checks if the specified character exists anywhere in the string.
 * Used to detect newline characters in the buffer for line boundary
 * detection in get_next_line.
 *
 * @param s: String to search in
 * @param c: Character to search for
 * @return: 1 if character found, 0 otherwise
 */
int	my_strchr(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

/*
 * Extracts a substring from a source string.
 * Creates a new string containing a portion of the source string
 * starting at the specified position with the specified length.
 * Handles boundary conditions and allocates memory for the result.
 *
 * @param s: Source string to extract from
 * @param start: Starting position for extraction
 * @param len: Maximum length of substring to extract
 * @return: Newly allocated substring, NULL on failure
 * Side effects: Allocates memory for the substring
 */
char	*my_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*buff;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= my_strlen(s))
		return (my_strdup(""));
	if (start + len > my_strlen(s))
		len = my_strlen(s) - start;
	buff = malloc(len + 1);
	if (!buff)
		return (NULL);
	while (s[start + i] && i < len)
	{
		buff[i] = s[start + i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}

/*
 * Calculates the length of a string.
 * Counts the number of characters in the string until the
 * null terminator is reached. Handles NULL pointer input
 * safely by returning 0.
 *
 * @param s: String to measure
 * @return: Length of the string, 0 if NULL
 */
size_t	my_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}
