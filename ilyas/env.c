#include "help.h"

// Free the linked list of t_env
void free_env_list(t_list **env)
{
    t_list *current;
    t_list *next;
    if(!env || !*env)
        return;
    
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

//Look for the "PATH" key in your linked list.
//Return its value if found, otherwise NULL.
char *get_path_value(t_list *env)
{
    while(env)
    {
        if(env->key && ft_strcmp(env->key, "PATH") == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}
//mazal khasni nzid xi functions li ghan7tajom f pipex.c


/*
 * Searches for an executable command in PATH directories.
 * Iterates through PATH directories, constructs full paths by joining
 * directory with command name, and tests each for executable access.
 * Returns the first valid executable path found.
 *
 * @param full_path: Pointer to store the found executable path
 * @param path_cmd: Array of PATH directory strings
 * @param p: Command name to search for
 * Side effects: Allocates memory for path construction,
 * modifies full_path pointer
 */
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

/*
 * Attempts to find command in current working directory.
 * Constructs a path by joining the current working directory with
 * the command name and checks if it's executable. Used when command
 * doesn't contain path separators but might be in current directory.
 *
 * @param p: Command name to search for in current directory
 * @return: Full path to executable if found and accessible, NULL otherwise
 * Side effects: Allocates memory for path construction, calls getcwd()
 */
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
	tmp = my_strjoin(tmp, p);
	if (!tmp)
		return (NULL);
	if (access(tmp, X_OK) == 0)
		return (tmp);
	free(tmp);
	return (NULL);
}

/*
 * Resolves command name to full executable path.
 * Main command resolution function that handles different path cases:
 * absolute/relative paths, current directory search, and PATH variable search.
 * Implements the shell's command lookup algorithm.
 *
 * @param p: Command name or path to resolve
 * @param env: Environment variables list for PATH lookup
 * @return: Full path to executable if found, NULL if not found
 * Side effects: Allocates memory for path strings, performs file system access
 */
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
		path_cmd = parsing_split(path, ':');// i need to add it in parsing_split.c
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
