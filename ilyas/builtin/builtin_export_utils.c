#include "../help.h"
/*
 * Creates a complete copy of an environment variable linked list.
 */
t_env *copy_env_list(t_env *env)
{
    t_env *copy = NULL;
    t_env *tmp = env;

    while (tmp)
    {
        add_env_node(&copy, strdup(tmp->key), strdup(tmp->value));
        tmp = tmp->next;
    }
    return copy;
}
/*
 * Searches for an existing environment variable by key name.
 */
t_env *find_env_var(const char *key, t_env *env)
{
    while(env)
    {
        if (ft_strcmp(env->key, key))
            return env;
        env = env->next;
    }
    return NULL;
}
/*
 * Validates shell variable identifier names according to shell naming rules.
 */
int is_valide_identifier(const char *str)
{
    int i = 0;
    if (!(ft_isalpha(str[0]) || str[0] == '_'))
        return 0;
    while(str[i])
    {
        if((!ft_isalnum(str[i]) || str[i] == '_'))
            return 0;
        i++;
    }
    return 1;
}