#include "../help.h"


/* ================================================================
 * builtin_env - Print environment variables in KEY=VALUE format
 * 
 * Mimics the behavior of the 'env' command by displaying all
 * environment variables. Special handling for '_' variable to
 * show '/usr/bin/env' as expected by shell standards.
 * 
 * @param env: Double pointer to environment variable list
 * @return: 0 on success, 1 on error (sets g_exit_status)
 ================================================================ */
 
int	builtin_env(t_list **env)
{
	t_list	*tmp;

	if (!env || !*env)
		return (g_exit_status = 1, 1);
	tmp = *env;
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			if (ft_strcmp(tmp->key, "_") == 0)
			{
				ft_putstr_fd("_=/usr/bin/env\n", 1);
			}
			else
			{
				ft_putstr_fd(tmp->key, 1);
				ft_putstr_fd("=", 1);
				ft_putstr_fd(tmp->value, 1);
				ft_putstr_fd("\n", 1);
			}
		}
		tmp = tmp->next;
	}
	return (g_exit_status = 0, 0);
}
