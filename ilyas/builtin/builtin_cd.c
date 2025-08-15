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
void update_pwd_vars(t_list **env, char *new_path, char *old_path)  
{
	char *cwd;

	set_env_var(env, "OLDPWD", old_path);  
	cwd = getcwd(NULL, 0);
	if(cwd)
	{
		set_env_var(env, "PWD", cwd);
		free(cwd);
	}
	free(new_path);
	free(old_path);  
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
char *expand_tilde(char *cmd, t_list *env)
{
	char *home;
	char *expanded;

	if(!cmd || cmd[0] != '~')
		return (ft_strdup(cmd));  
	
	home = get_env_value(env, "HOME");
	if(!home)
		return (NULL);
	
	if(cmd[1] == '\0')
		return (ft_strdup(home));

	expanded = malloc(ft_strlen(home) + ft_strlen(cmd));  
	if(!expanded)
		return NULL;
	
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
int resolve_cd_target(char **cmd, char **path, char *old_path, t_list *env)
{
	char *tmp;

	if(!cmd[1])
	{
		tmp = get_env_value(env, "HOME");
		if(!tmp)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			free(old_path);
			return(1);
		}
		*path = ft_strdup(tmp);
	}
	else if(ft_strcmp(cmd[1], "-") == 0)
	{
		tmp = get_env_value(env, "OLDPWD");  
		if(!tmp)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");  
			free(old_path);
			return(1);
		}
		*path = ft_strdup(tmp);
		printf("%s\n", *path);  
	}
	else if (cmd[1][0] == '~')
	{
		*path = expand_tilde(cmd[1], env);
		if (!*path) 
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			free(old_path);
			return(1);
		}
	}
	else 
		*path = ft_strdup(cmd[1]);
	
	
	if (!*path)
	{
		free(old_path);
		return(1);
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
int builtin_cd(char **cmd, t_shell *shell)
{
    char *old_path;
    char *new_path;
    
    if(cmd[1] && cmd[2])
    {
        fprintf(stderr, "minishell: cd: too many arguments\n");
        return(1);
    }
    
    old_path = getcwd(NULL, 0);
    if (!old_path)
    {
        perror("minishell: cd");  
        return 1;
    }
    
    if(resolve_cd_target(cmd, &new_path, old_path, shell->env) != 0)
        return 1;
        
    if(chdir(new_path) != 0)
    {
        perror("minishell: cd");
        free(old_path);
        free(new_path);
        return(1);
    }
    
    update_pwd_vars(&(shell->env), new_path, old_path);  
    return 0;
}