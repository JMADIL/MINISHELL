/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:15:37 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 12:15:38 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	safe_free(char *s)
{
	if (s)
		free(s);
}

void	print_error_exit(const char *cmd_name, const char *error, int status)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (cmd_name)
	{
		write(STDERR_FILENO, cmd_name, ft_strlen((char *)cmd_name));
		write(STDERR_FILENO, ": ", 2);
	}
	if (error)
		write(STDERR_FILENO, error, ft_strlen((char *)error));
	write(STDERR_FILENO, "\n", 1);
	_exit(status);
}

int	safe_open(const char *file, int flag)
{
	int	fd;

	if (!file)
		return (-1);
	if (flag == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else if (flag == 2)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
	{
		if (errno == ENOENT)
			print_error_exit("no sush file or directory", file, 1);
		else if (errno == EACCES)
			print_error_exit("permission denied\n", file, 1);
		else if (errno == EISDIR)
			print_error_exit("is a directory\n", file, 1);
		else
			print_error_exit("cannot open file\n", file, 1);
	}
	return (fd);
}

void	execve_error_cleanup(char **cmd_path, char **cmd_name, char **envp)
{
	int	i;

	if (cmd_path && *cmd_path)
		safe_free(*cmd_path);
	if (cmd_name && *cmd_name)
		safe_free(*cmd_name);
	if (envp)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
		free(envp);
	}
	if (errno == EACCES)
		print_error_exit(*cmd_name, "permission denied", 126);
	else
		print_error_exit(*cmd_name, "command not found", 127);
}
