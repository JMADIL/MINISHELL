#include "../help.h"
/*
 * Validates if a string represents a valid numeric value.
 * Checks for optional sign followed by digits only. Handles edge cases
 * like empty strings, sign-only strings, and mixed characters.
 *
 * Algorithm: Character-by-character validation with sign handling
 * Time complexity: O(n) where n is string length
 *
 * @param str: String to validate for numeric content
 * @return: 1 if valid numeric string, 0 if invalid or NULL
 * Side effects: None - read-only operation
 */
int numeric(const char *str)
{
    int i = 0;
    
    if (!str || !str[0])
        return 0;
    
    if (str[0] == '+' || str[0] == '-')
        i++;
    
    if (!str[i])  
        return 0;
        
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

/*
 * Implements the exit builtin command functionality.
 * Handles exit with optional numeric argument, validates input,
 * manages exit status modulo 256, and handles multiple arguments.
 * Follows bash exit behavior for error cases and status codes.
 *
 * Algorithm: Argument validation with proper exit status handling
 * - No args: exit with last command status
 * - Numeric arg: exit with that status (mod 256)
 * - Non-numeric: exit with status 255
 * - Too many args: return error without exiting
 *
 * @param cmd: Command array (cmd[0]="exit", cmd[1]=optional status)
 * @param env: Environment list to free before exit
 * @return: 1 if too many arguments (doesn't exit), otherwise exits process
 * Side effects: Prints "exit", may print errors, frees environment, exits process
 */
int builtin_exit(char **cmd, t_list *env)
{
    long exit_code;
    
    write(1, "exit\n", 5);
    
    if (!cmd[1])
    {
        free_env_list(&env);  
        exit(g_exit_status);
    }
    
    if (!numeric(cmd[1]))  
    {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cmd[1]);
        free_env_list(&env);
        exit(255);
    }
    
    if (cmd[2])
    {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        return 1;  
    }
    
    exit_code = ft_atol(cmd[1]);  
    exit_code = exit_code % 256;  
    if (exit_code < 0)
        exit_code += 256;  
    
    free_env_list(&env);
    exit((int)exit_code);
}