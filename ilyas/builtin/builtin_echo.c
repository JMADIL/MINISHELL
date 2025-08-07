#include "../help.h"

/*
 * Validates that a string contains only 'n' characters after "-".
 * Used to verify that echo -n flags contain only 'n' characters,
 * ensuring proper parsing of the no-newline option.
 *
 * @param str: String to validate (should start with "-n")
 * @return: 1 if string contains only 'n' after "-", 0 otherwise
 */
int	is_vaid_n_flag(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/*
 * Joins command arguments into a single string with spaces.
 *
 * @param cmd: Array of command arguments
 * @param i: Starting index for joining arguments
 * @return: Joined string containing all arguments separated by spaces
 */

char	*join_args_from_index(char **cmd, int i)
{
	char	*result;
	size_t	total_len;
	int		j;
	int		k;

	total_len = 0;
	j = i;
	k = 0;
	if (!cmd || !cmd[i])
		return (ft_strdup(""));
	while (cmd[j])
	{
		total_len = ft_strlen(cmd[j]);
		if (cmd[j + 1])
			total_len++;
		j++;
	}
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (cmd[i])
	{
		ft_strlcat(result, cmd[i]);
		if (cmd[i + 1])
			ft_strlcat(result, " ");
		i++;
	}
	return (result);
}


/*
 * Outputs the echo string with or without a trailing newline.
 *
 * @param tmp: String to output
 * @param n_flag: If non-zero, suppresses trailing newline
 */

void prent_echo_output(const char *tmp, int n_flag)
{
	if (tmp)
		write(STDOUT_FILENO, tmp, ft_strlen(tmp));
	if(!n_flag)
		write(STDOUT_FILENO, "\n", 1);
}
/*
 * Implements the echo builtin command.
 * Outputs the specified arguments to stdout, supporting the -n option.
 *
 * @param cmd: Array of command arguments (cmd[0] is "echo")
 * @param shell: (unused)
 * @return: Always returns 0
 */
int builtin_echo(char **cmd)
{
	int i = 1;
	int n_flag = 0;
	char *joined;

	while(cmd[i] && is_valid_n_flag(cmd[i]))
	{
		n_flag = 1;
		i++;
	}
	joined = join_args_from_index(cmd, i);
	if(!joined)
		return 1;
	print_echo_output(!joined, n_flag);
	free(joined);
	return 0;
}