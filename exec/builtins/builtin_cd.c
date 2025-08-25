/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:06 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 14:30:18 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_pwd_vars(t_list *env, char *new_path, char *old_path)
{
	char	*cwd;
	t_list	*old_pwd;
	t_list	*current_pwd;
	char	*tmp;

	safe_free(new_path);
	tmp = NULL;
	old_pwd = find_node(env, "OLDPWD");
	current_pwd = find_node(env, "PWD");
	cwd = getcwd(NULL, 0);
	if (old_pwd)
	{
		if (old_pwd->value)
			free(old_pwd->value);
		old_pwd->value = ft_strdup(old_path);
	}
	free(old_path);
	if (current_pwd)
	{
		tmp = current_pwd->value;
		current_pwd->value = cwd;
		free(tmp);
	}
	else
		free(cwd);
}

char	*expand_tilde(char *cmd, t_list *env)
{
	char	*home;
	char	*expanded;

	if (!cmd || cmd[0] != '~')
		return (ft_strdup(cmd));
	home = ft_getenv("HOME", env);
	if (!home)
		return (NULL);
	if (cmd[1] == '\0')
		return (ft_strdup(home));
	expanded = malloc(ft_strlen(home) + ft_strlen(cmd));
	if (!expanded)
		return (NULL);
	ft_strcpy(expanded, home);
	ft_strcat(expanded, cmd + 1);
	return (expanded);
}

int	resolve_cd_target(char **cmd, char **path, char *old_path, t_list **env)
{
	char	*tmp;

	tmp = NULL;
	if (!cmd[1] || ft_strcmp(cmd[1], "~") == 0)
	{
		*path = ft_getenv("HOME", *env);
		if (!*path)
			CD_ERROR_RETURN(old_path, CD_HOME_ERROR);
	}
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		*path = ft_getenv("OLDPWD", *env);
		if (!*path)
			CD_ERROR_RETURN(old_path, CD_OLDPWD_ERROR);
	}
	else
	{
		tmp = expand_tilde(cmd[1], *env);
		if (!tmp)
			CD_ERROR_RETURN(old_path, CD_HOME_ERROR);
		*path = ft_strdup(tmp);
		if (tmp != cmd[1])
			free(tmp);
	}
	return (0);
}

int	builtin_cd(char **cmd, t_list **env)
{
	char	*old_path;
	char	*new_path;

	if (cmd[1] && cmd[2])
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		return (1);
	}
	old_path = getcwd(NULL, 0);
	if (!old_path)
	{
		perror("minishell: cd");
		return (1);
	}
	if (resolve_cd_target(cmd, &new_path, old_path, env) != 0)
		return (1);
	if (chdir(new_path) != 0)
	{
		perror("minishell: cd");
		free(old_path);
		free(new_path);
		return (1);
	}
	update_pwd_vars(*env, new_path, old_path);
	return (0);
}
