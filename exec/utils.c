/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:25:34 by irfei             #+#    #+#             */
/*   Updated: 2025/08/30 11:57:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	size_list(t_cmdarg *node)
{
	int	count;

	count = 0;
	if (!node)
		return (0);
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count); 
}

void	free_all(char **bf, int j)
{
	while (j >= 0)
	{
		free(bf[j]);
		bf[j] = NULL;
		j--;
	}
	free(bf);
}

void	ft_alloc(char **envp, int *i, char *key_equals, t_list *env)
{
	char	*str;

	while (env)
	{
		if (ft_strcmp(env->key, "SHLVL") == 0)
			env->value = ft_itoa(ft_atoi(env->value) + 1);
		key_equals = ft_strjoin(env->key, "=");
		str = ft_strjoin(key_equals, env->value);
		free(key_equals);
		envp[*i] = ft_strdup(str);
		free(str);
		env = env->next;
		(*i)++;
	}
}

char	**get_env(t_list *env)
{
	char	**envp;
	int		env_count;
	t_list	*current_env;
	char	*key_equals;

	env_count = 0;
	key_equals = NULL;
	current_env = env;
	if (!env)
		return (NULL);
	while (current_env)
	{
		env_count++;
		current_env = current_env->next;
	}
	envp = malloc(sizeof(char *) * (env_count + 1));
	if (!envp)
		return (NULL);
	env_count = 0;
	ft_alloc(envp, &env_count, key_equals, env);
	envp[env_count] = NULL;
	return (envp);
}

void	ft_free_isdir(char **cmd_path, char **cmd_name, t_cmdarg *current_cmd)
{
	free(*cmd_path);
	free(*cmd_name);
	print_error_exit(current_cmd->cmd[0], "is a directory\n", 126);
}
