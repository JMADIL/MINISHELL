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
void	append_to_env_value(t_list *dup_key, char *key, char *value)
{
	char	*new_val;

	if (!dup_key)
	{
		if (key)
			free(key);
		if (value)
			free(value);
		return ;
	}
	if (key)
		free(key);
	if (dup_key->value && value)
	{
		new_val = malloc(ft_strlen(dup_key->value) + ft_strlen(value) + 1);
		if (!new_val)
		{
			free(value);
			return ;
		}
		ft_strcpy(new_val, dup_key->value);
		ft_strcat(new_val, value);
		free(dup_key->value);
		free(value);
		dup_key->value = new_val;
	}
	else if (value)
	{
		dup_key->value = value;
	}
	dup_key->check = 1;
}

/*
 * Parses export command string into key/value.
 * Handles VAR=value and VAR+=value
 * Returns 1 if append operation, 0 if normal, -1 if invalid
 */
int	parse_export_argument(const char *cmd, char **key, char **value)
{
	char *plus_pos;
	char *eq_pos;

	if (!cmd || !*cmd || !key || !value)
		return (-1);

	*key = NULL;
	*value = NULL;

	plus_pos = ft_strstr(cmd, "+=");
	eq_pos = ft_strchr(cmd, '=');

	if (plus_pos && (!eq_pos || plus_pos < eq_pos))
	{
		*key = ft_substr((char *)cmd, 0, plus_pos - cmd);
		*value = ft_strdup(plus_pos + 2);
		if (!*key || !*value)
		{
			if (*key)
			{
				free(*key);
				*key = NULL;
			}
			if (*value)
			{
				free(*value);
				*value = NULL;
			}
			return (-1);
		}
		return (1);
	}
	else if (eq_pos)
	{
		*key = ft_substr((char *)cmd, 0, eq_pos - cmd);
		*value = ft_strdup(eq_pos + 1);
		if (!*key || !*value)
		{
			if (*key)
			{
				free(*key);
				*key = NULL;
			}
			if (*value)
			{
				free(*value);
				*value = NULL;
			}
			return (-1);
		}
		return (0);
	}
	else
	{
		handle_export_no_assign(key, value, cmd);
		if (!*key)
			return (-1);
		return (0);
	}
}