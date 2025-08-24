/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:26 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:28:33 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_list	*find_env_var(const char *key, t_list *env)
{
	t_list	*curr;

	if (!key || !env)
		return (NULL);
	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

static void	process_export_variable(const char *cmd, char **key, char **value,
		t_list **env)
{
	int		append_flag;
	t_list	*dup_key;

	*key = NULL;
	*value = NULL;
	append_flag = parse_export_argument(cmd, key, value);
	if (append_flag == -1 || !*key)
		return ;
	dup_key = find_env_var(*key, *env);
	if (dup_key)
	{
		if (append_flag == 1)
			append_to_env_value(dup_key, key, value);
		else
			update_existing_env(dup_key, key, value, (char *)cmd);
	}
	else
		ft_lstadd_back(env, ft_lstnew(*key, *value));
}

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!((str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z')
			|| str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z')
				|| (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0'
					&& str[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

int	validate_export_identifier(const char *cmd)
{
	int		len;
	char	*tmp;

	if (!cmd || cmd[0] == '\0')
		return (print_export_identifier_error(""));
	if (cmd[0] == '=')
		return (print_export_identifier_error(cmd));
	len = 0;
	while (cmd[len] && cmd[len] != '=' && !(cmd[len] == '+' && 
			cmd[len + 1] == '='))
		len++;
	if (len == 0)
		return (print_export_identifier_error(cmd));
	tmp = ft_substr(cmd, 0, len);
	if (!tmp)
		return (1);
	if (!is_valid_identifier(tmp))
	{
		free(tmp);
		return (print_export_identifier_error(cmd));
	}
	free(tmp);
	return (0);
}

int	builtin_export(char **cmd, t_list **env)
{
	int		error_flag;
	char	*key;
	char	*value;

	error_flag = 0;
	if (!cmd[1])
	{
		print_sorted_export(env);
		return (0);
	}
	while (*cmd)
	{
		if (validate_export_identifier(*cmd) != 0)
			error_flag = 1;
		process_export_variable(*cmd, &key, &value, env);
		cmd++;
	}
	return (error_flag);
}
