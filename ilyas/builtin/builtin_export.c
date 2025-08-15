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
t_list *find_env_var(const char *key, t_list *env)
{
    t_list *curr;
    if(!key || !env)
        return NULL;
    curr = env;
    while(curr)
    {
        if(ft_strcmp(curr->key, key) == 0)
            return (curr);
        curr = curr->next;

    }
    return NULL;
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
static void process_export_variable(const char *cmd, char **key, char **value, t_list **env)
{
    int append_flag;
    t_list *dup_key;

    *key = NULL;
    *value = NULL;  

    append_flag = parse_export_argument(cmd, key, value);
    if(append_flag == -1 || !*key)
        return ;
    dup_key = find_env_var(*key, *env);
    if(dup_key)
    {
        if(append_flag == 1)
            append_to_env_value(dup_key, *key, *value);
        else
            update_existing_env_var(dup_key, *key, *value, cmd);
    }
    else
        add_new_env_var(env, *key, *value);
}

/*
 * Validates export command variable identifier format.
 * Checks if variable names follow shell identifier rules: must start with
 * letter or underscore, followed by alphanumeric characters or underscores.
 * Handles both regular assignments (VAR=value) and append operations (VAR+=value).
 *
 * Algorithm: String validation with character-by-character checking
 * - Extract variable name portion (before '=' or '+=')
 * - Validate first character (letter or underscore only)
 * - Validate remaining characters (alphanumeric or underscore)
 * Time complexity: O(n) where n is variable name length
 *
 * @param cmd: Export command argument to validate
 * @return: 0 if valid identifier, 1 if invalid (with error message)
 * Side effects: Prints error messages, allocates temporary memory
 */
int validate_export_identifier(const char *cmd)
{
    int i;
    int len;
    
    if (!cmd || cmd[0] == '\0')
        return (print_export_identifier_error(""));
    if (cmd[0] == '=')
        return (print_export_identifier_error(cmd));
    
    len = 0;
    while (cmd[len] && cmd[len] != '=' && !(cmd[len] == '+' && cmd[len + 1] == '='))
        len++;
    
    if (len == 0)
        return (print_export_identifier_error(cmd));
    
    if (!((cmd[0] >= 'A' && cmd[0] <= 'Z') ||
          (cmd[0] >= 'a' && cmd[0] <= 'z') ||
          cmd[0] == '_'))
        return (print_export_identifier_error(cmd));
    
    i = 1;
    while (i < len) 
    {
        if (!(cmd[i] == '_' ||
              (cmd[i] >= 'A' && cmd[i] <= 'Z') ||
              (cmd[i] >= 'a' && cmd[i] <= 'z') ||
              (cmd[i] >= '0' && cmd[i] <= '9')))
            return (print_export_identifier_error(cmd));
        i++;
    }
    
    return (0);
}

/*
* Implements the export builtin command functionality.
* Processes export command with optional arguments to set/modify environment
* variables. Without arguments, displays all exported variables in sorted format.
* With arguments, validates and processes each variable assignment or declaration.
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
* Side effects: Modifies environment list, prints variables/errors, processes all args
*/
int	builtin_export(char **cmd, t_list **env)
{
	int		i;
	int		error_flag;
	char	*key;
	char	*value;

	error_flag = 0;
	if (!cmd[1])
	{
		print_sorted_export(*env);
		return (0);
	}
	i = 1;
	while (cmd[i])
	{
		if (validate_export_identifier(cmd[i]) != 0)
		{
			error_flag = 1;
			i++;
			continue ;
		}
		key = NULL;
		value = NULL;
		process_export_variable(cmd[i], &key, &value, env);
		i++;
	}
	return (error_flag);
}