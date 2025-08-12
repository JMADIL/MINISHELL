#include "../help.h"


/*
 * Creates a complete copy of an environment variable linked list.
 * Duplicates all nodes with their key-val pairs to create an independent cpy
 * that can be modified without affecting the original environment list.
 * Used primarily for sorting environment variables for display.
 *
 * @param env: Source environment linked list to copy
 * @return: New linked list copy, NULL if source is NULL or memory alloc fails
 * Side effects: Allocates memory for new list nodes and strings
 */

/*
 * Searches for an existing environment variable by key name.
 * Performs case-sensitive string comparison to find matching environment
 * variable in the linked list. Used to check for duplicate keys before
 * adding new variables or when updating existing ones.
 *
 * @param key: Environment variable name to search for
 * @param env: Environment variables linked list to search in
 * @return: Pointer to matching list node, NULL if not found
 */
t_env *copy_env_list(t_env *env)
{
    t_env *copy = NULL;
    t_env *tmp = env;

    while(tmp)
    {
        add_env_node(&copy, ft_strdup(tmp->key), ft_strdup(tmp->value));
        tmp = tmp->next;
    }
    return copy;
}

/*
 * Allocates and processes key-value pairs for export command arguments.
 * Handles parsing of export arguments including append operations (VAR+=value)
 * and regular assignments (VAR=value). Manages duplicate key detection and
 * either updates existing variables or creates new ones.
 *
 * @param cmd: Export command argument string
 * (e.g., "VAR=value" or "VAR+=append")
 * @param key: Pointer to store extracted variable key
 * @param value: Pointer to store extracted variable value
 * @param env: Environment variables linked list to modify
 * Side effects: May modify environment list, allocates memory for key/value
 */

 