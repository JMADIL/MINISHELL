/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:23 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 02:13:24 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../help.h"

/*
 * Prints error message for invalid export variable identifiers.
 * Example: minishell: export: `1abc`: not a valid identifier
 */
int	print_export_identifier_error(const char *cmd)
{
	if (!cmd)
		return (1);
	write(2, "minishell: export: `", 20);
	write(2, cmd, ft_strlen((char *)cmd));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

/*
 * Helper for variable without assignment: export VAR
 * Allocates key, sets value to NULL
 */
void	handle_export_no_assign(char **key, char **value, const char *cmd)
{
	if (!key || !value || !cmd)
	{
		if (key)
			*key = NULL;
		if (value)
			*value = NULL;
		return ;
	}
	*key = ft_strdup((char *)cmd);
	*value = NULL;
}

/*
 * Handles append operation: VAR+=value
 * If variable exists, appends value; if not, sets new value.
 */
void	append_to_env_value(t_list *dup_key, char **key, char **value)
{
	char	*old_value;

	if (!dup_key || !key || !value || !*value)
		return ;
	old_value = dup_key->value;
	if (!old_value)
		dup_key->value = ft_strdup(*value);
	else
	{
		dup_key->value = ft_strjoin(old_value, *value);
		free(old_value);
	}
	free(*key);
	free(*value);
	*key = NULL;
	*value = NULL;
	dup_key->check = 1;
}

/*
 * Parses export command string into key/value.
 * Handles VAR=value and VAR+=value
 * Returns 1 if append operation, 0 if normal, -1 if invalid
 */

/* ================================================================
 * parse_assignment_type - Determine assignment type and extract key/value
 ================================================================ */
static int	parse_assignment_type(const char *cmd, char **key, char **value)
{
	char	*plus_pos;
	char	*eq_pos;

	plus_pos = ft_strchr(cmd, '+');
	eq_pos = ft_strchr(cmd, '=');
	if (plus_pos && *(plus_pos + 1) == '=')
	{
		*key = ft_substr((char *)cmd, 0, plus_pos - cmd);
		*value = ft_strdup(plus_pos + 2);
		return (1);
	}
	else if (eq_pos)
	{
		alloc_key_value((char *)cmd, key, value, eq_pos);
		return (0);
	}
	else
	{
		handle_export_no_assign(key, value, cmd);
		return (0);
	}
}

/* ================================================================
 * parse_export_argument - Parse export command argument
 ================================================================ */
int	parse_export_argument(const char *cmd, char **key, char **value)
{
	int	result;

	if (!cmd || !*cmd || !key || !value)
		return (-1);
	*key = NULL;
	*value = NULL;
	result = parse_assignment_type(cmd, key, value);
	if (!*key)
	{
		if (*value)
			free(*value);
		return (-1);
	}
	return (result);
}
