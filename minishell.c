/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:12:33 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/24 19:43:52 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*user_input;

	setup_interactive_signals();
	(void)av;
	if (ac != 1)
		return (printf("Error: No arguments expected\n"), 1);
	else
	{
		minienv = ft_initenv(env);
		while (1337)
		{
			user_input = readline("minishell-1.0$ ");
			if (!user_input)
				print_error_exit(NULL,  "exit", 0);
			minishell(user_input, &minienv);
			free(user_input);
		}
	}
	return (0);
}
