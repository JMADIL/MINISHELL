/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tmp_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 09:56:26 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 10:07:47 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int create_tmp_heredoc(void)
{
    int fd;

    // Create a unique temporary file inside /tmp
    fd = open("/tmp/.heredoc_tmp", O_CREAT | O_RDWR, 0600);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}
