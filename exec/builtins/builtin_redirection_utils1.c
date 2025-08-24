/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redirection_utils1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:34 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:10:23 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
