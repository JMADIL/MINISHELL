#include "../minishell.h"

/*
 * Initializes and finds the last input redirection (< or <<) in the list
 */
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

/*
 * Initializes and finds the last output redirection (> or >>) in the list
 */
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

/*
 * Initializes redirection metadata for input and output redirections.
 * Marks last redirection in the list and resets file descriptors.
 */

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

/*
 * Checks if heredoc input should end (EOF or matching delimiter)
 */
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

/*
 * Reads heredoc input until delimiter is encountered
 */
void	read_heredoc_input_gnl(char *delim, int fd_pipe[2],
		t_redi_list *heredoc, t_list *env)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (is_heredoc_end(line, delim))
			break ;
		if (heredoc->expand)
			expand_var_in_char(&line, env);
		if (heredoc->is_last)
			heredoc->content = ft_strjoin(heredoc->content, line);
		free(line);
		line = NULL;
	}
	if (heredoc->content && heredoc->is_last)
	{
		heredoc->heredoc_fd = create_tmp_heredoc();
		write(heredoc->heredoc_fd, heredoc->content, ft_strlen(heredoc->content));
	}
	close(heredoc->heredoc_fd);
	free(heredoc->content);
	heredoc->content = NULL;
	close(fd_pipe[1]);
}
