/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:17 by irfei             #+#    #+#             */
/*   Updated: 2025/08/23 03:37:30 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
/*
 * Validates if a string represents a valid numeric value.
 * Checks for optional sign followed by digits only. Handles edge cases
 * like empty strings, sign-only strings, and mixed characters.
 *
 * Algorithm: Character-by-character validation with sign handling
 * Time complexity: O(n) where n is string length
 *
 * @param str: String to validate for numeric content
 * @return: 1 if valid numeric string, 0 if invalid or NULL
 * Side effects: None - read-only operation
 */
int numeric(const char *str)
{
    int i = 0;
    
    if (!str || !str[0])
        return 0;
    
    if (str[0] == '+' || str[0] == '-')
        i++;
    
    if (!str[i])  
        return 0;
        
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}


/* ================================================================
 * handle_exit_error - Handle numeric argument error and exit
 * 
 * Prints error message for non-numeric arguments and exits with
 * status 255 after cleaning up environment.
 * 
 * @param cmd: Command array (for error message)
 * @param env: Environment list to free before exit
 ================================================================ */
static void	handle_exit_error(char **cmd, t_list **env)
{
	fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cmd[1]);
	free_env_list(env);
	exit(255);
}

/* ================================================================
 * builtin_exit - Handle exit builtin command
 * 
 * Processes exit command with optional numeric argument.
 * Validates arguments, handles errors, and exits with appropriate
 * status code (0-255 range).
 * 
 * @param cmd: Command array [exit, arg1, arg2, ...]
 * @param env: Environment list to free before exit
 * @return: 1 if too many arguments (doesn't exit), otherwise exits
 ================================================================ */
int	builtin_exit(char **cmd, t_list **env)
{
	long	exit_code;

	write(1, "exit\n", 5);
	if (cmd[1] && cmd[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	if (cmd[1])
	{
		if (!numeric(cmd[1]))
			handle_exit_error(cmd, env);
		exit_code = ft_atoi(cmd[1]);
		exit_code = exit_code % 256;
		if (exit_code < 0)
			exit_code += 256;
		g_exit_status = (int)exit_code;
	}

	free_env_list(env);
	exit(g_exit_status);
}

