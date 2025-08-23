/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:47 by irfei             #+#    #+#             */
/*   Updated: 2025/08/23 12:13:29 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Frees a null-terminated array of strings (double pointer).
 * Iterates through the array, freeing each individual string before
 * freeing the array pointer itself. Used for cleaning up command
 * argument arrays and other string arrays.
 *
 * @param cmd: Null-terminated array of strings to free
 * Side effects: Frees all allocated memory for strings and array
 */
void	free_arr(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "unset") == 0);
}

int setup_builtin_io(t_cmdarg *shell, int *input_fd, int *output_fd)
{
    *input_fd = 0;  
    *output_fd = 1; 
    
    // Handle input redirection
    if (shell->input)
    {
        t_redi_list *input_redir = shell->input;
        
        if (input_redir->type == INPUT) // <
        {
            // if (!input_redir->next || !input_redir->next->file)
            // {
            //     write(2, "minishell: syntax error near unexpected token\n", 47);
            //     return (-1);
            // }
            *input_fd = open(input_redir->next->file, O_RDONLY);
            if (*input_fd == -1)
            {
                perror(input_redir->file);
                return (-1);
            }
        }
        else if (input_redir->type == HEREDOC) // <<
        {
            if (input_redir->heredoc_fd != -1)
                *input_fd = input_redir->heredoc_fd;
            else if (input_redir->tmp_fd != -1)
                *input_fd = input_redir->tmp_fd;
        }
    }
    
    // Handle output redirection
    if (shell->output)
    {
        t_redi_list *output_redir = shell->output;
        
        if (output_redir->type == OUTPUT) // >
        {
            // if (!output_redir->next || !output_redir->next->file)
            // {
            //     write(2, "minishell: syntax error near unexpected token\n", 47);
            //     if (*input_fd != STDIN_FILENO)
            //         close(*input_fd);
            //     return (-1);
            // }
            *output_fd = open(output_redir->next->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*output_fd == -1)
            {
                perror(output_redir->file);
                if (*input_fd != STDIN_FILENO)
                    close(*input_fd);
                return (-1);
            }
        }
        else if (output_redir->type == APPEND) // >>
        {
            // if (!output_redir->next || !output_redir->next->file)
            // {
            //     write(2, "minishell: syntax error near unexpected token\n", 47);
            //     if (*input_fd != STDIN_FILENO)
            //         close(*input_fd);
            //     return (-1);
            // }
            *output_fd = open(output_redir->next->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*output_fd == -1)
            {
                perror(output_redir->file);
                if (*input_fd != STDIN_FILENO)
                    close(*input_fd);
                return (-1);
            }
        }
    }
    
    return (0);
}

// Function to cleanup file descriptors
void cleanup_builtin_io(int input_fd, int output_fd)
{
    if (input_fd != STDIN_FILENO)
        close(input_fd);
    if (output_fd != STDOUT_FILENO)
        close(output_fd);
}

int	exec_builtin(t_cmdarg *shell, t_list **env)
{
	int	result;
    int input_fd;
    int output_fd;
    
    // Setup I/O redirection for builtins
    if (setup_builtin_io(shell, &input_fd, &output_fd) == -1)
    {
        g_exit_status = 1;
        return (1);
    }


    if (ft_strcmp(shell->cmd[0], "cd") == 0)
        result = builtin_cd(shell->cmd, env);
    else if (ft_strcmp(shell->cmd[0], "echo") == 0)
        result = builtin_echo(shell->cmd, shell, output_fd);
    else if (ft_strcmp(shell->cmd[0], "env") == 0)
        result = builtin_env(env, output_fd);
    else if (ft_strcmp(shell->cmd[0], "exit") == 0)
        result = builtin_exit(shell->cmd, env);
    else if (ft_strcmp(shell->cmd[0], "export") == 0)
        result = builtin_export(shell->cmd, env);
    else if (ft_strcmp(shell->cmd[0], "pwd") == 0)
        result = builtin_pwd(env, output_fd);
    else if (ft_strcmp(shell->cmd[0], "unset") == 0)
        result = builtin_unset(shell->cmd, env);
    else
    {
        // Cleanup file descriptors before returning for non-builtin
        cleanup_builtin_io(input_fd, output_fd);
        return (1);
    }
    
    // Cleanup file descriptors
    cleanup_builtin_io(input_fd, output_fd);
	g_exit_status = result;
	return (result);
}
