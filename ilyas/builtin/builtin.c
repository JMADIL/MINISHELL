#include "../help.h"

int is_builtin(char *cmd)
{
	if(!cmd)
		return 0;
	return (
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "env") == 0||
		ft_strcmp(cmd, "exit") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0
	);
}
int exec_builtin(t_cmdarg *cmd, t_shell *shell)
{
    if (!cmd || !cmd->cmd || !cmd->cmd[0])  // Use cmd->cmd instead of cmd->argv
        return(1);
        
    if(ft_strcmp(cmd->cmd[0], "cd") == 0)  
        return (builtin_cd(cmd->cmd, shell));  // Pass cmd->cmd
    else if(ft_strcmp(cmd->cmd[0], "echo") == 0)
        return(builtin_echo(cmd->cmd));  // Remove shell parameter if builtin_echo doesn't need it
    else if(ft_strcmp(cmd->cmd[0], "env") == 0)
        return(builtin_env(shell->env));  // Pass shell->env
    else if(ft_strcmp(cmd->cmd[0], "exit") == 0)
        return(builtin_exit(cmd->cmd, shell->env));  // Pass shell->env
    else if(ft_strcmp(cmd->cmd[0], "export") == 0)
        return(builtin_export(cmd->cmd, shell));
    else if(ft_strcmp(cmd->cmd[0], "unset") == 0)
        return(builtin_unset(cmd->cmd, shell));
    return(1);
}