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
int exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return(1);
	if(ft_strcmp(cmd->argv[0], "cd" == 0))
		return (builtin_cd(cmd->argv, shell));
	else if(ft_strcmp(cmd->argv[0], "echo" == 0))
		return(builtin_echo(cmd->argv, shell));
	else if(ft_strcmp(cmd->argv[0], "env" == 0))
		return(builtin_env(cmd->argv, shell));
	else if(ft_strcmp(cmd->argv[0], "exit"))
		return(builtin_exit(cmd->argv, shell));
	else if(ft_strcmp(cmd->argv[0], "export"))
		return(builtin_export(cmd->argv, shell));
	else if(ft_strcmp(cmd->argv[0], "unset"))
		return(builtin_unset(cmd->argv, shell));
	return(1);
	
}