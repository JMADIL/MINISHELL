#include "../help.h"

// ghadi nsta3melha f minishell.c ya3ni n9dar nbedela blasa
bool	dontexpand_heredoc_del(t_token *tmp)
{
	if (!tmp || !tmp->prev)
		return (false);
	if (tmp->prev->type != HEREDOC)
		return (false);
	if (tmp->type == WORD || tmp->type == DOUBLE_QUOTE)
		return (true);
	return (false);
}


void	handle_file_open_error(char *filename)
{
	if (errno == ENOENT)
		display_redi_error(filename, "No such file or directory");
	else if (errno == EISDIR)
		display_redi_error(filename, "Is a directory");
	else if (errno == EACCES)
		display_redi_error(filename, "Permission denied");
	else
		display_redi_error(filename, "Cannot open file");
}



/*
 * Checks if redirect target is ambiguous
 */

// i use this function in pipex_utils.c
bool	check_ambiguous_redirect(char *filename)
{
	char	*tmp;

	if (!filename || ft_strlen(filename) == 0)
		return (true);
	tmp = filename;
	while (*tmp)
	{
		if (*tmp == ' ')
			return (true);
		tmp++;
	}
	return (false);
}

