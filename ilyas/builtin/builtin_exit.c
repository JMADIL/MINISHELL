#include "help.h"
int numeric(const char *str)
{
    int i = 0;

    if(!str || !str[0])
        return 0;
    if(str[0] == '+' || str[0] == '-')
        i++;
    while(str[i])
    {
        if(str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}
/*
 * Implements the exit builtin command functionality.
 */

 int builtin_exit(char **cmd, t_env *env)
 {
    int exit_code;
    write(1, "exit\n", 5);
    if(!cmd[1])
    {
        free_env_list(env);
        exit(g_exit_status);
    }
    if(!is_numeric(cmd[1]))
    {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cmd[1]);
        free_env_list(env);
        exit(255);
    }
    if(cmd[2])
    {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        return 1;
    }
    exit_code = ft_atoi(cmd[1]) % 256;
    free_env_list(env);
    exit(exit_code);
 }