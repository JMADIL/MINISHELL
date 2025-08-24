/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 02:13:20 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 08:08:25 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
		else
			return ;
	}
}

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
