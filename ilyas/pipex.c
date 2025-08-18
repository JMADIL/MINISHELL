#include "help.h"

/*
 * Checks if a given path points to a directory.
 * Uses stat system call to determine if the path exists and represents
 * a directory. Only checks paths that contain forward slashes and are
 * not empty or null.
 *
 * @param path: File path string to check
 * @return: true if path is a directory, false otherwise
 */

bool	is_directory(const char *path)
{
	struct stat	st;

	if (!path || *path == '\0')
		return (false);
	if (!ft_strchr(path, '/'))
		return (false);
	if (stat(path, &st) == -1)
		return (false);
	return (S_ISDIR(st.st_mode));
}
static void	exec_external_command(t_cmdarg *current_cmd, char **env)
{
	char *cms
}