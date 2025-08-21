#include "../help.h"

void add_new_env_var(t_list **env, char *key, char *value)
{
    ft_lstadd_back(env, ft_lstnew(key, value));
}

/*
 * Prints environment variables in export format.
 * Displays each environment variable in the "declare -x" format used by
 * the export builtin. Handles variables with null values by omitting the
 * assignment, and properly quotes values containing spaces or special chars.
 *
 * @param head: Environment variables linked list to print
 * Side effects: Prints formatted environment variables to stdout
 */

void	print_export_format(t_list *head)
{
	while (head)
	{
		if (head->key)
		{
			printf("declare -x %s", head->key);
			if (head->value != NULL)
				printf("=\"%s\"", head->value);
			printf("\n");
			head = head->next;
		}
		head = head->next;
	}
}
/*
 * Swaps two adjacent nodes in environment list during sorting.
 * Compares keys alphabetically and swaps both key and value pointers
 * if the first key is lexicographically greater than the second.
 * Part of the bubble sort implementation for environment display.
 *
 * @param ptr1: First node in the pair to potentially swap
 * Side effects: May swap key and value pointers between adjacent nodes
 */

void	swap_env_nodes(t_list *ptr1)
{
	char	*tmp_key;
	char	*tmp_val;
	int		tmp_check;

	if (!ptr1 || !ptr1->next)
		return ;
	tmp_key = ptr1->key;
	tmp_val = ptr1->value;
	tmp_check = ptr1->check;
	ptr1->key = ptr1->next->key;
	ptr1->value = ptr1->next->value;
	ptr1->check = ptr1->next->check;
	ptr1->next->key = tmp_key;
	ptr1->next->value = tmp_val;
	ptr1->next->check = tmp_check;
}

/*
 * Prints environment variables in sorted export format.
 * Creates a copy of the environment list, sorts it alphabetically by key
 * using bubble sort algorithm, prints in "declare -x" format,
	then frees memory.
 *
 * Algorithm: Bubble sort - repeatedly passes through the list, swapping
 * adjacent elements that are out of order until no swaps are needed.
 * Time complexity: O(n²), Space complexity: O(n) for the copy.
 *
 * @param env: Pointer to environment variables linked list
 * Side effects: Prints to stdout, allocates/frees memory for temporary copy
 */
void	print_sorted_export(t_list **env)
{
	t_list	*copy;
	t_list	*head;
	t_list	*ptr;
	int		list_size;

	if (!env || !*env)
		return ;
	copy = copy_env_list(*env);
	if (!copy)
		return ;
	head = copy;
	list_size = ft_lstsize(copy);
	while (list_size > 0)
	{
		ptr = head;
		while (ptr && ptr->next)
		{
			if (ft_strcmp(ptr->key, ptr->next->key) > 0)
				swap_env_nodes(ptr);
			ptr = ptr->next;
		}
		list_size--;
	}
	print_export_format(head);
	free_env_list(&head);
}

/*
 * Handles memory allocation when updating existing environment variables.
 * Manages memory for duplicate keys in export operations, properly freeing
 * old values and setting new ones. Handles both assignment and declaration
 * cases based on whether the command contains an '=' character.
 *
 * @param dup_key: Existing environment node to update
 * @param key: Pointer to key string (will be freed)
 * @param value: Pointer to value string (may be used or freed)
 * @param cmd: Original command string to check for assignment
 * Side effects: Modifies environment node, frees memory
 */

/*
 * Updates an existing environment variable with new value.
 * Handles both assignment (VAR=value) and declaration (VAR) cases.
 * For assignments, replaces the old value with new one and sets check flag.
 * For declarations without '=', keeps existing value and updates check flag.
 *
 * Algorithm: String parsing and conditional memory management
 * - If '=' present: update value and set check=1
 * - If no '=': preserve existing value, set check based on value existence
 *
 * @param dup_key: Existing environment node to update
 * @param key: Key string to free (no longer needed)
 * @param value: New value string (ownership transferred or freed)
 * @param cmd: Original command string to check for '=' presence
 * Side effects: Modifies dup_key, frees memory, updates check flag
 */
void	update_existing_env(t_list *dup_key, char **key, char **value,
		char *cmd)
{
	if (!dup_key || !key || !*key)
		return ;
	free(*key);
	*key = NULL;
	if (ft_strchr(cmd, '='))
	{
		if (dup_key->value)
			free(dup_key->value);
		dup_key->value = *value;
		*value = NULL;
		dup_key->check = 1;
	}
	else
	{
		if (*value)
		{
			free(*value);
			*value = NULL;
		}
		if (!dup_key->value)
			dup_key->check = 0;
		else
			dup_key->check = 1;
	}
}
