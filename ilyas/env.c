#include "help.h"

// Free the linked list of t_env
void free_env_list(t_env **list)
{
    t_env *current;
    t_env *next;
    if(!list || !*list)
        return;
    
    current = *list;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    *list = NULL;
    
}

//Look for the "PATH" key in your linked list.
//Return its value if found, otherwise NULL.
char *get_path_value(t_env *env)
{
    while(env)
    {
        if(env->key && strcmp(env->key, "PATH") == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}
