/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:12:33 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/08 05:58:38 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			->>minishell();<<-
			free(user_input);
		}
	}
	return (0);
}