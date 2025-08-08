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