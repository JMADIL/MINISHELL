
#include "help.h"

/*
 * Counts the number of nodes in a command argument linked list.
 * Iterates through the linked list to determine the total number
 * of command nodes in the pipeline. Used for pipeline management
 * and memory allocation calculations.
 *
 * @param node: Head of the command argument linked list
 * @return: Number of nodes in the list, 0 if list is NULL
 */
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

/*
 * Frees an array of strings up to a specified index.
 * Iterates backwards from the given index, freeing each string
 * and setting pointers to NULL, then frees the array itself.
 * Used for cleanup when array allocation partially fails.
 *
 * @param bf: Array of strings to free
 * @param j: Last valid index to free from (inclusive)
 * Side effects: Frees memory for strings and array
 */
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

/*
 * Allocates and populates environment variable strings for execve.
 * Converts the internal environment linked list format to the string
 * array format required by execve. Handles special case of SHLVL
 * increment and formats each variable as "KEY=VALUE".
 *
 * @param envp: Array to populate with environment strings
 * @param i: Pointer to current index in array
 * @param key_equals: Temporary string pointer (unused parameter)
 * @param env: Environment variables linked list
 * Side effects: Allocates memory for environment strings, modifies SHLVL value
 */
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

/*
 * Converts environment linked list to string array for execve.
 * Creates a null-terminated array of environment variable strings
 * in the format required by execve system call. Each string follows
 * the "KEY=VALUE" format expected by external programs.
 *
 * @param env: Environment variables linked list
 * @return: Null-terminated array of env strings, NULL on alloc failure
 * Side effects: Allocates memory for string array and individual strings
 */
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
	ft_cmd_error(current_cmd->cmd[0], "is a directory\n", 126);
}
