#include "help.h"

int builtin_env(t_env *env)
{
    t_env *tmp;

    if(!env)
        return 1;
    tmp = env;
    while(tmp)
    {
        if(tmp->value)
        {
            if(ft_strcmp(tmp->key, "_") == 0)
                ft_putstr_fd("_=/usr/bin/env\n");
            else
            {
                ft_putstr_fd(tmp->key, 1);
                ft_putstr_fd("_", 1);
                ft_putstr_fd(tmp->value, 1);
                ft_putstr_fd("\n", 1);
            }
        }
        tmp = tmp->next;
    }
    g_exit_status = 0;
    return 1;
}