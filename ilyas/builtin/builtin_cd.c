/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:06 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 02:46:37 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../help.h"

/*
 * Updates PWD and OLDPWD environment variables after directory change.
 * Sets OLDPWD to the previous working directory and PWD to the current
 * working directory. Handles cases where environment variables may not exist.
 *
 * @param env: Environment variables linked list
 * @param new_path: Path string to free (from target path resolution)
 * @param old_path: Previous working directory path
 * Side effects: Modifies environment variables, allocates/frees memory,
 * calls getcwd
 */
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

/*
 * Handles tilde expansion for cd command paths.
 * Expands paths beginning with '~' by replacing the tilde with the
 * HOME environment variable value. Returns the original command if
 * no tilde expansion is needed.
 *
 * @param cmd: Command path that may contain tilde
 * @param env: Environment variables list for HOME lookup
 * @return: Expanded path string or original command, NULL if HOME not set
 * Side effects: May allocate memory for expanded path
 */
char	*expand_tilde(char *cmd, t_list *env)
{
	char	*home;
	char	*expanded;

	if (!cmd || cmd[0] != '~')
		return (ft_strdup(cmd));
	home = ft_getenv(env, "HOME"); // parsing function
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

/*
 * Determines the target directory path for the cd command.
 * Handles various cd cases: no argument (HOME), '-' (OLDPWD), tilde expansion,
 * and regular paths. Validates that required environment variables exist
 * and handles error cases appropriately.
 *
 * @param cmd: Command array containing cd and its arguments
 * @param path: Pointer to store the resolved target path
 * @param old_path: Current working directory path for error cleanup
 * @param env: Pointer to environment variables list
 * @return: 0 on success, 1 on error
 * Side effects: Allocates memory for path, may write error messages,
 * frees old_path on error
 */
int	resolve_cd_target(char **cmd, char **path, char *old_path, t_list **env)
{
	char	*tmp;

	tmp = NULL;
	if (!cmd[1] || ft_strcmp(cmd[1], "~") == 0)
	{
		*path = ft_getenv(env, "HOME");// parsing amealem
		if (!*path)
			CD_ERROR_RETURN(old_path, CD_HOME_ERROR);
	}
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		*path = ft_getenv(env, "OLDPWD");
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

/*
 * Implements the cd builtin command.
 * Changes the current working directory to the specified path, handling
 * special cases like HOME directory, previous directory (-),
 * and tilde expansion.
 * Updates PWD and OLDPWD environment variables and validates arguments.
 *
 * @param cmd: Command array where cmd[0] is "cd" and cmd[1] is target directory
 * @param shell: Pointer to shell structure containing environment variables
 * @return: 0 on success, 1 on error
 * Side effects: Changes working directory, modifies environment variables,
 * may write errors
 */
int	builtin_cd(char **cmd, t_list **env)
{
	char	*old_path;
	char	*new_path;

	if (cmd[1] && cmd[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
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
