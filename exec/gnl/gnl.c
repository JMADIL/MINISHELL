/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:14:08 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 12:03:13 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*resit(char *hold)
{
	int		i;
	char	*ret;

	i = 0;
	if (!hold)
		return (NULL);
	while (hold[i])
	{
		if (hold[i] == '\n')
		{
			ret = ft_substr(hold, i + 1, ft_strlen(hold) - i);
			free(hold);
			hold = NULL;
			return (ret);
		}
		i++;
	}
	return (NULL);
}

char	*befor_newline(char **hold)
{
	int		i;
	char	*line;
	char	*current;

	i = 0;
	current = *hold;
	if (hold && current)
	{
		while (current[i])
		{
			if (current[i] == '\n')
				return (ft_substr(current, 0, i + 1));
			i++;
		}
		if (current[i] == '\0')
		{
			line = ft_substr(current, 0, i);
			free(current);
			*hold = NULL;
			return (line);
		}
	}
	return (NULL);
}

/*
 * Reads data from file descriptor and accumulates it in buffer.
 * Continuously reads chunks of data from the file descriptor
 * until a newline is found or end of file is reached. Joins
 * read data with existing buffer content.
 *
 * @param fd: File descriptor to read from
 * @param hold: Existing buffer content to append to
 * @return: Accumulated buffer with read data, NULL on error
 * Side effects: Allocates memory for accumulated data, reads from fd
 */
static char	*get_line(int fd, char *hold)
{
	char	buff[BUFFER_SIZE + 1];
	int		rd;
	char	*tmp;

	rd = 1;
	while (rd > 0)
	{
		rd = read(fd, buff, BUFFER_SIZE);
		if (rd < 0)
		{
			free(hold);
			hold = NULL;
			return (NULL);
		}
		buff[rd] = '\0';
		tmp = ft_strjoin(hold, buff);
		free(hold);
		hold = tmp;
		if (!hold || ft_strchr(hold, '\n'))
			break ;
	}
	return (hold);
}

/*
 * Reads the next line from a file descriptor.
 * Main function that manages the complete line reading process.
 * Handles buffer management, line extraction, and proper cleanup.
 * Uses static buffer to maintain state between calls.
 *
 * @param fd: File descriptor to read from
 * @return: Next complete line from file, NULL at EOF or error
 * Side effects: Maintains static buffer, allocates memory for returned line
 */
char	*get_next_line(int fd)
{
	static char	*hold = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(hold);
		hold = NULL;
		return (NULL);
	}
	hold = get_line(fd, hold);
	if (!hold)
		return (NULL);
	line = befor_newline(&hold);
	if (!line)
	{
		free(hold);
		hold = NULL;
		return (NULL);
	}
	hold = resit(hold);
	return (line);
}
