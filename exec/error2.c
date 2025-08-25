/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:17:15 by irfei             #+#    #+#             */
/*   Updated: 2025/08/25 12:18:41 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finish_exec(pid_t last_cmd_pid)
{
	int	status;

	setup_parent_wait_signals();
	wait_for_all_children(&status, last_cmd_pid);
	restore_interactive_signals();
}
