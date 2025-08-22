/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:13:26 by irfei             #+#    #+#             */
/*   Updated: 2025/08/22 02:13:27 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../help.h"

/*
 * Searches for an existing environment variable by key name.
 * Traverses the environment variables linked list to find a node
 * with a matching key. Uses string comparison to check for exact matches.
 * Returns pointer to the found node or NULL if not found.
 *
 * Algorithm: Linear search through linked list
 * Time complexity: O(n) where n is number of environment variables
 * Space complexity: O(1) - only uses temporary pointer
 *
 * @param key: Variable name to search for (e.g., "PATH", "HOME")
 * @param env: Head of environment variables linked list
 * @return: Pointer to matching environment node, or NULL if not found
 * Side effects: None - read-only operation
 */
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

/*
 * Processes and adds/updates environment variables from export command.
 * Parses command string to extract key-value pairs, handles both regular
 * assignments (VAR=value) and append operations (VAR+=value). Updates
 * existing variables or creates new ones in the environment list.
 *
 * Algorithm: String parsing with duplicate checking and conditional updates
 * - Parse command for assignment type (regular/append/declaration)
 * - Check if variable already exists in environment
 * - Update existing or create new environment node accordingly
 * Time complexity: O(n) for duplicate search, Space complexity: O(1)
 *
 * @param cmd: Export command string (e.g., "VAR=value", "VAR+=append")
 * @param key: Pointer to store extracted variable key (modified)
 * @param value: Pointer to store extracted value (modified)
 * @param env: Pointer to environment variables list (may be modified)
 * Side effects: Modifies environment list, allocates memory, may free strings
 */
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

/*
 * Validates shell variable identifier names according to shell naming rules.
 * Checks that variable names start with letter or underscore, contain only
 * alphanumeric characters and underscores.
 * Ensures compliance with POSIX shell variable naming conventions.
 *
 * @param str: Variable identifier string to validate
 * @return: 1 if valid identifier, 0 if invalid
 */
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

/*
 * Validates export command arguments and reports syntax errors.
 * Checks for proper variable identifier format, handles special cases like
 * empty arguments and invalid characters. Extracts variable name portion
 * for validation regardless of assignment or append operations.
 *
 * @param cmd: Export command argument to validate
 * @return: 0 if valid, 1 if error (with error message printed)
 * Side effects: Writes error messages to stderr, may allocate temporary memory
 */
int	validate_export_identifier(const char *cmd)
{
	int		len;
	char	*tmp;

	if (!cmd || cmd[0] == '\0')
		return (print_export_identifier_error(""));
	if (cmd[0] == '=')
		return (print_export_identifier_error(cmd));
	len = 0;
	while (cmd[len] && cmd[len] != '=' && !(cmd[len] == '+' && cmd[len
			+ 1] == '='))
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

/*
 * Implements the export builtin command functionality.
 * Processes export command with optional arguments to set/modify environment
 * variables. Without arguments,
	displays all exported variables in sorted format.
 * With arguments,
	validates and processes each variable assignment or declaration.
 *
 * Algorithm: Argument parsing with validation and batch processing
 * - If no arguments: display sorted environment variables
 * - For each argument: validate format, then process assignment
 * - Continue processing all arguments even if some fail (collect errors)
 * Time complexity: O(n*m) where n=arguments, m=env_size, Space complexity: O(1)
 *
 * @param cmd: Array of command arguments (cmd[0] is "export")
 * @param env: Pointer to environment variables list
 * @return: 0 if all operations successful, 1 if any errors occurred
 * Side effects: Modifies environment list, prints variables/errors,
	processes all args
 */
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
