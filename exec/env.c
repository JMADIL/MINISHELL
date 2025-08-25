/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:16:36 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 12:16:37 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// z3ma get_cmd_path

#include "../minishell.h"

void	free_env_list(t_list **env)
{
	t_list	*current;
	t_list	*next;

	if (!env || !*env)
		return ;
	current = *env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}

char	*get_path_value(t_list *env)
{
	while (env)
	{
		if (env->key && ft_strcmp(env->key, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	ft_get_path(char **full_path, char **path_cmd, char *p)
{
	char	*tmp;

	tmp = NULL;
	*full_path = NULL;
	while (*path_cmd)
	{
		*full_path = ft_strjoin(*path_cmd, "/");
		if (!*full_path)
			return ;
		tmp = *full_path;
		*full_path = ft_strjoin(*full_path, p);
		free(tmp);
		tmp = NULL;
		if (!*full_path)
			return ;
		if (access(*full_path, X_OK) == 0)
			return ;
		free(*full_path);
		*full_path = NULL;
		path_cmd++;
	}
}

char	*ft_join_with_path(char *p)
{
	char	*path;
	char	*tmp;

	tmp = NULL;
	path = getcwd(NULL, 0);
	if (!path)
		return (NULL);
	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		free(path);
		return (NULL);
	}
	free(path);
	tmp = ft_strjoin(tmp, p);
	if (!tmp)
		return (NULL);
	if (access(tmp, X_OK) == 0)
		return (tmp);
	free(tmp);
	return (NULL);
}

char	*check_exec(char *p, t_list *env, int *no_file)
{
	char	*full_path;
	char	*path;
	char	**path_cmd;
	char	*dir_path;

	full_path = validate_exec_path(p);
	if (full_path)
		return (full_path);
	path = get_path_value(env);
	if (path)
	{
		path_cmd = split_with_braces(path, ':');
		if (!path_cmd)
			return (NULL);
		ft_get_path(&full_path, path_cmd, p);
		free_arr(path_cmd);
		if (full_path)
			return (full_path);
	}
	else
		*no_file = 1;
	dir_path = ft_join_with_path(p);
	if (dir_path)
		return (dir_path);
	return (NULL);
}
