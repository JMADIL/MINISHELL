#include "help.h"

// Split PATH, try all combinations dir/command, check access
char *search_command_in_path(char *cmd, char *path_value)
{
    char **paths;
    char *full_path;
    int i;

    if(!path_value || !cmd)
        return NULL;
    
    paths = ft_split(path_value, ';');
    if(!paths)
        return NULL;
    
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin_free(full_path, cmd);
        
        if(access(full_path, X_OK) == 0)
        {
            ft_free_split(paths);
            return full_path;
        }
        free(full_path);
        i++;
    }
    ft_free_split(paths);
    return NULL;

}

//Wraps all the above — implements shell resolution logic
char *resolve_command_path(char *cmd, t_list *env)
{
	char *path_value;
	char *full_path;

	if(!cmd)
		return NULL;
	
	if(ft_stchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd);
		return NULL;
	}
	path_value = get_path_value(env);
	full_path = search_command_in_path(cmd, path_value);
	return full_path;

}

//call
//maghadix nb9a khdem biha
void xec_eternal_command(t_cmdarg *cmd, t_shell *shell)  
{
    pid_t pid;
    char *path;
    char **envp;
    
    path = resolve_command_path(cmd->cmd[0], shell->env); 
    if(!path)
    {
        write(2, "minishell: command not found\n", 29);
        shell->last_status = 127;
        return;
    }
    pid = fork();
    if(pid == -1)
    {
        perror("fork");
        free(path);
        return;
    }
    if(pid == 0)
    {
        envp = env_to_array(shell->env);
        execve(path, cmd->cmd, envp);  
        perror("execve");
        ft_free_split(envp);
        free(path);
        exit(1);
    }
    else
    {
        waitpid(pid, &shell->last_status, 0);
        if(WIFEXITED(shell->last_status))
            shell->last_status = WEXITSTATUS(shell->last_status);
        free(path);
    }
}


