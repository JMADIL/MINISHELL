/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:19:21 by irfei             #+#    #+#             */
/*   Updated: 2025/08/28 19:18:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redi_list	*get_last_input_redirection(t_redi_list *redi)
{
	t_redi_list	*last_input;

	last_input = NULL;
	while (redi)
	{
		redi->tmp_fd = -1;
		redi->is_last = false;
		redi->heredoc_fd = -1;
		redi->content = NULL;
		if (redi->type == INPUT || redi->type == HEREDOC)
			last_input = redi;
		redi = redi->next;
	}
	if (last_input)
		last_input->is_last = true;
	return (last_input);
}

t_redi_list	*get_last_output_redirection(t_redi_list *redi)
{
	t_redi_list	*last_output;

	last_output = NULL;
	while (redi)
	{
		redi->tmp_fd = -1;
		redi->is_last = false;
		redi->heredoc_fd = -1;
		redi->content = NULL;
		if (redi->type == OUTPUT || redi->type == APPEND)
			last_output = redi;
		redi = redi->next;
	}
	if (last_output)
		last_output->is_last = true;
	return (last_output);
}

void	init_redirection_metadata(t_cmdarg *cmd)
{
	t_redi_list	*last_input;
	t_redi_list	*last_output;
	t_cmdarg	*tmp;

	if (!cmd)
		return ;
	tmp = cmd;
	while (tmp)
	{
		last_input = get_last_input_redirection(tmp->redirections);
		last_output = get_last_output_redirection(tmp->redirections);
		if (last_input)
			last_input->is_last = true;
		if (last_output)
			last_output->is_last = true;
		tmp = tmp->next;
	}
}

int	is_heredoc_end(char *line, const char *delimiter)
{
	if (!line)
	{
		g_exit_status = 0;
		return (1);
	}
	if (delimiter && ft_strlen(line) - 1 == ft_strlen((char *)delimiter)
		&& ft_strncmp(line, delimiter, ft_strlen((char *)delimiter)) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

void	rd_hr_in_gl(char *delim, int fd_pipe[2], t_redi_list *herc, t_list *env)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (is_heredoc_end(line, delim))
			break ;
		if (herc->expand)
			expand_var_in_char(&line, env);
		if (herc->is_last)
			herc->content = ft_strjoin(herc->content, line);
		free(line);
		line = NULL;
	}
	if (herc->content && herc->is_last)
	{
		herc->heredoc_fd = create_tmp_heredoc2();
		write(herc->heredoc_fd, herc->content, ft_strlen(herc->content));
	}
	close(herc->heredoc_fd);
	free(herc->content);
	herc->content = NULL;
	close(fd_pipe[1]);
}
