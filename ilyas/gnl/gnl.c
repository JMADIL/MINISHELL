
#include "get_next_line.h"

/*
 * Extracts remaining content after a newline character.
 * After finding and returning a line, this function preserves
 * any remaining content in the buffer for the next call to
 * get_next_line. Handles memory management for the static buffer.
 *
 * @param hold: Static buffer containing read data
 * @return: Remaining content after newline, NULL if no remainder
 * Side effects: Frees original buffer, allocates new buffer for remainder
 */
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
			ret = my_substr(hold, i + 1, my_strlen(hold) - i);
			free(hold);
			hold = NULL;
			return (ret);
		}
		i++;
	}
	return (NULL);
}

/*
 * Extracts a complete line from the buffer up to and including newline.
 * Searches for a newline character in the buffered data and extracts
 * the complete line including the newline. If no newline is found,
 * returns the entire remaining content as the final line.
 *
 * @param hold: Pointer to static buffer containing read data
 * @return: Complete line including newline, or remaining content
 * Side effects: May modify or free the static buffer
 */
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
				return (my_substr(current, 0, i + 1));
			i++;
		}
		if (current[i] == '\0')
		{
			line = my_substr(current, 0, i);
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
		hold = my_strjoin(hold, buff);
		if (!hold || my_strchr(hold, '\n'))
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
