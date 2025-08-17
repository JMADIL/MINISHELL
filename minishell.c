/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:12:33 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/17 17:37:15 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(char *input, t_list **minienv)
{
	t_token		*token_list;
	t_cmdarg	*cmdarg_list;
	
	if(input[0] == '\0')
		return ;
	add_history(input);
	if (!ft_quote_checker(input))
    	return ;
	token_list = ft_strtok(input, *minienv);
	if (->>ft_check_syntax(token_list)<<- == -1)
	//ft_check_syntax scans the tokens to detect invalid command structures.
    	return (->>ft_free_tokenlist(token_list)<<-);
	//Converts the flat token list into a higher-level structure: t_cmdarg
	cmdarg_list = ->>ft_parser(token_list, *minienv);<<-
	//Handle here-documents (<<) :
	if (!->>check_here_doc(cmdarg_list, *minienv)<<-)
    	return (->>ft_cleaner(token_list, cmdarg_list)<<-);
	//Run builtin commands (no fork) :
	if(->>check_builtin(cmdarg_list, minienv)<<- == 1)
		return (->>ft_cleaner(token_list, cmdarg_list)<<-);
	//Execute non-builtin commands :
	if (!execution(cmdarg_list, *minienv))
    	return (ft_cleaner(token_list, cmdarg_list));
	//Cleanup after command execution :
	ft_cleaner(token_list, cmdarg_list);
}

int main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*user_input;
	
	->>handle_signals();<<-
	(void)av;
	if(ac != 1)
		return (printf("\nError: No arguments expected\n"), 1);
	else
	{
		minienv = ft_initenv(env);
		while(1337)
		{
			user_input = readline("minishell-1.0$ ");
			if(!user_input)
				->>ft_error_cmd();<<-
			minishell(user_input, &minienv);
			free(user_input);
		}
	}
	return (0);
}
