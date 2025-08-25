/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:25:34 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 14:25:36 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	size_list(t_cmdarg *node)
{
	int	i;

	i = 0;
	if (!node)
		return (0);
	while (node)
	{
		i++;
		node = node->next;
	}
	return (i);
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
	int		i;
	t_list	*tmp;
	char	*key_equals;

	i = 0;
	key_equals = NULL;
	tmp = env;
	if (!env)
		return (NULL);
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return (NULL);
	i = 0;
	ft_alloc(envp, &i, key_equals, env);
	envp[i] = NULL;
	return (envp);
}

void	ft_free_isdir(char **cmd_path, char **cmd_name, t_cmdarg *current_cmd)
{
	free(*cmd_path);
	free(*cmd_name);
	print_error_exit(current_cmd->cmd[0], "is a directory\n", 126);
}
