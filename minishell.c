/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:12:33 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/24 05:35:41 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int			g_exit_status = 0;

int	ft_check_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (ft_toksize(token_list) == 1 && ft_isredi(current))
		return (printf("syntax error near unexpected token `newline'\n"),
				g_exit_status = 258, -1);
	while (current)
	{
		if (ft_pipeerrors(current))
			return (printf("syntax error near unexpected token `|'\n"),
				g_exit_status = 258, -1);
		if (ft_redierrors(current))
			return (printf("syntax error near unexpected token `newline'\n"),
				g_exit_status = 258, -1);
		current = current->next;
	}
	return (0);
}

t_cmdarg	*parser(t_token *token_list, t_list	*minienv)
{
	t_cmdarg	*cmdarg_list;
	t_cmdarg	*node;
	t_token	*tmp;

	if(!token_list)
		return (NULL);
	cmdarg_list = NULL;
	tmp = token_list;
	while(tmp)
	{
		if(!please_expand(tmp))
			expand_variables(&tmp, minienv);
		tmp = tmp->next;
	}
	token_list->current = token_list;
	node = get_next_node(token_list);
	while(node)
	{
		if(node)
			ft_nodeadd_back(&cmdarg_list, ft_newnode(node));
		//this free function is in ft_split.c
		frees(node->cmd, node->cmdsize);
		free(node);
		node = get_next_node(token_list);
	}
	return (cmdarg_list);
}

void	minishell(char *input, t_list **minienv)
{
	t_token		*token_list;
	t_cmdarg	*cmdarg_list;

	if (input[0] == '\0')
		return ;
	add_history(input);
	if (!ft_quote_checker(input))
		return ;
	token_list = ft_strtok(input, *minienv);
	if (ft_check_syntax(token_list) == -1)
		// ft_check_syntax scans the tokens to detect invalid command structures.
		return (ft_free_tokenlist(token_list));
	// Converts the flat token list into a higher-level structure: t_cmdarg
	cmdarg_list = parser(token_list, *minienv);
		// Handle here-documents (<<) :
	if (!process_all_heredocs(cmdarg_list, *minienv)) 
		return (ft_cleaner(token_list, cmdarg_list));
	// Run builtin commands (no fork) :
	if (!exec_builtin(cmdarg_list, minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	// Execute non-builtin commands :
	if (!execution(cmdarg_list, *minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	// Cleanup after command execution :
	ft_cleaner(token_list, cmdarg_list);
}
// void	minishell(char *input, t_list **minienv)
// {
// 	t_token		*token_list;
// 	t_cmdarg	*cmdarg_list;

// 	printf("DEBUG: minishell() called with: '%s'\n", input);
	
// 	if (input[0] == '\0')
// 	{
// 		printf("DEBUG: Empty input, returning\n");
// 		return ;
// 	}
	
// 	printf("DEBUG: Adding to history\n");
// 	add_history(input);
	
// 	printf("DEBUG: Checking quotes\n");
// 	if (!ft_quote_checker(input))
// 	{
// 		printf("DEBUG: Quote check failed\n");
// 		return ;
// 	}
	
// 	printf("DEBUG: Tokenizing input\n");
// 	token_list = ft_strtok(input, *minienv);
// 	if (!token_list)
// 	{
// 		printf("DEBUG: Tokenization failed\n");
// 		return ;
// 	}
// 	printf("DEBUG: Tokenization successful\n");
	
// 	printf("DEBUG: Checking syntax\n");
// 	if (ft_check_syntax(token_list) == -1)
// 	{
// 		printf("DEBUG: Syntax check failed\n");
// 		return (ft_free_tokenlist(token_list));
// 	}
// 	printf("DEBUG: Syntax check passed\n");
	
// 	printf("DEBUG: Parsing tokens to cmdarg\n");
// 	cmdarg_list = parser(token_list, *minienv);
// 	if (!cmdarg_list)
// 	{
// 		printf("DEBUG: Parser failed\n");
// 		return (ft_free_tokenlist(token_list));
// 	}
// 	printf("DEBUG: Parser successful\n");
	
// 	printf("DEBUG: Processing heredocs\n");
// 	if (!process_all_heredocs(cmdarg_list, *minienv))
// 	{
// 		printf("DEBUG: Heredoc processing failed\n");
// 		return (ft_cleaner(token_list, cmdarg_list));
// 	}
// 	printf("DEBUG: Heredocs processed successfully\n");
	
// 	printf("DEBUG: About to check if builtin\n");
// 	if (cmdarg_list && cmdarg_list->cmd && cmdarg_list->cmd[0])
// 		printf("DEBUG: Command is: '%s'\n", cmdarg_list->cmd[0]);
// 	else
// 		printf("DEBUG: WARNING: cmdarg_list or cmd is NULL!\n");
	
// 	printf("DEBUG: Executing builtin check\n");
// 	if (exec_builtin(cmdarg_list, minienv) == 1)
// 	{
// 		printf("DEBUG: Builtin executed, cleaning up\n");
// 		return (ft_cleaner(token_list, cmdarg_list));
// 	}
// 	printf("DEBUG: Not a builtin, proceeding to external execution\n");
	
// 	printf("DEBUG: Executing external command\n");
// 	if (!execution(cmdarg_list, *minienv))
// 	{
// 		printf("DEBUG: External execution failed\n");
// 		return (ft_cleaner(token_list, cmdarg_list));
// 	}
// 	printf("DEBUG: External execution successful\n");
	
// 	printf("DEBUG: Cleaning up\n");
// 	ft_cleaner(token_list, cmdarg_list);
// 	printf("DEBUG: minishell() function completed\n");
// }

int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*user_input;

	setup_interactive_signals();
	(void)av;
	if (ac != 1)
		return (printf("\nError: No arguments expected\n"), 1);
	else
	{
		minienv = ft_initenv(env);
		while (1337)
		{
			user_input = readline("minishell-1.0$ ");
			if (!user_input)
				print_error_exit(NULL,  "[EOF]\n", 0);
			minishell(user_input, &minienv);
			free(user_input);
		}
	}
	return (0);
}
// int	main(int ac, char **av, char **env)
// {
// 	t_list	*minienv;
// 	char	*user_input;

// 	printf("DEBUG: Starting main\n");
// 	setup_interactive_signals();
// 	printf("DEBUG: Signals set up\n");
	
// 	(void)av;
// 	if (ac != 1)
// 		return (printf("\nError: No arguments expected\n"), 1);
	
// 	minienv = ft_initenv(env);
// 	printf("DEBUG: Environment initialized\n");
	
// 	while (1337)
// 	{
// 		printf("DEBUG: About to call readline\n");
// 		user_input = readline("minishell-1.0$ ");
// 		printf("DEBUG: Readline returned\n");
		
// 		if (!user_input)
// 			print_error_exit(NULL,  "[EOF]\n", 0);
		
// 		printf("DEBUG: About to call minishell\n");
// 		minishell(user_input, &minienv);
// 		printf("DEBUG: minishell returned\n");
		
// 		free(user_input);
// 	}
// 	return (0);
// }
