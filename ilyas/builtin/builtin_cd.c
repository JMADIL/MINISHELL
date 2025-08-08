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
void update_pwd_vers(t_env **env, char *new_path, char *old_path)
{
	char *cwd;

	set_env_var(env, "OLDPWN", old_path);
	cwd = getcwd(NULL, 0);
	if(cwd)
	{
		set_env_var(env, "PWD", cwd);
		free(cwd);
	}
	free(new_path);

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
char *expand_tilde(char *cmd, t_env *env)
{
	char *home;
	char *expanded;

	if(!cmd || cmd[0] != '~')
		return (cmd);
	home = get_env_value(env, "HOME");
	if(!home)
		return (NULL);
	if(cmd[1] == '\0')
		return (ft_strdup(home));

	expanded = malloc(ft_strlen(home) + ft_strlen(cmd));
	if(!expanded)
	{
		return NULL;
	}
}