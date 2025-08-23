/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:28 by irfei             #+#    #+#             */
/*   Updated: 2025/08/23 11:30:26 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Implements the pwd builtin command.
 * Prints the current working directory path to stdout using the getcwd
 * system call. Handles errors by printing an error message via perror.
 *
 * @param env: Pointer to environment variables list (unused)
 * @return: Always returns 1
 * Side effects: Writes current directory to stdout, may write error to stderr
 */
int	builtin_pwd(t_list **env, int out)
{
	char *cwd;

	cwd = ft_getenv("PWD", *env); // parsing ajemi
	if (cwd != NULL)
	{
		write(out, cwd, ft_strlen(cwd));
		write(out, "\n",1);
		free(cwd);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}
