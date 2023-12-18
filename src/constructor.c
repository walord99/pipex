/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 14:55:19 by bplante           #+#    #+#             */
/*   Updated: 2023/12/18 15:02:00 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	allocate_pipe_tab(t_pipe_tab *pipe_tab, int size)
{
	pipe_tab->pipes = ft_calloc((size) * 2 + 1, sizeof(int));
	pipe_tab->len = size;
}
