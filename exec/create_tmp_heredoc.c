/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tmp_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 09:56:26 by irfei             #+#    #+#             */
/*   Updated: 2025/08/24 19:14:55 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int create_tmp_heredoc(void)
{
    int fd;

    // Create a unique temporary file inside /tmp
    fd = open("/tmp/.heredoc_tmp", O_CREAT | O_RDWR, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}

int create_tmp_heredoc2(void)
{
    int fd;

    // Create a unique temporary file inside /tmp
    fd = open("/tmp/.heredoc_tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}
