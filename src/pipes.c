/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 02:23:11 by bplante/Wal       #+#    #+#             */
/*   Updated: 2023/12/18 14:59:05 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_all_pipes(t_pipe_tab *pipe_tab)
{
	int	i;

	i = 1;
	while (i < pipe_tab->len - 1)
	{
		close(pipe_tab->pipes[i * 2 + READ]);
		close(pipe_tab->pipes[i * 2 + WRITE]);
		i++;
	}
	close(pipe_tab->pipes[0 + READ]);
	close(pipe_tab->pipes[(pipe_tab->len - 1) * 2 + WRITE]);
}

int	create_pipes_tab(t_pipe_tab *pipe_tab, int argc, char **argv)
{
	allocate_pipe_tab(pipe_tab, argc - 2);
	if (generate_pipes(pipe_tab) == -1)
	{
		free(pipe_tab->pipes);
		return (2);
	}
	pipe_tab->pipes[0 + READ] = open(argv[1], O_RDONLY);
	if (pipe_tab->pipes[0 + READ] == -1)
	{
		pipe_tab->pipes[0 + READ] = open("/dev/null", O_RDONLY);
		ft_printf_fd("pipex: %s: %s\n", 2, argv[1], strerror(errno));
	}
	pipe_tab->pipes[0 + WRITE] = NOT_PIPE;
	pipe_tab->pipes[(pipe_tab->len - 1) * 2 + READ] = -2;
	pipe_tab->pipes[(pipe_tab->len - 1) * 2 + WRITE] = open(argv[argc - 1],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipe_tab->pipes[(pipe_tab->len - 1) * 2 + WRITE] == -1)
	{
		pipe_tab->pipes[(pipe_tab->len - 1) * 2 + WRITE] = open("/dev/null",
				O_WRONLY);
		ft_printf_fd("pipex: %s: %s\n", 2, argv[argc - 1], strerror(errno));
		return (1);
	}
	return (0);
}

int	generate_pipes(t_pipe_tab *pipes_tab)
{
	int	i;

	i = 1;
	while (i < pipes_tab->len - 1)
	{
		if (pipe(&pipes_tab->pipes[i * 2]) == -1)
			break ;
		i++;
	}
	if (i != pipes_tab->len - 1)
	{
		i--;
		while (i >= 0)
		{
			close(pipes_tab->pipes[i * 2 + WRITE]);
			close(pipes_tab->pipes[i * 2 + READ]);
			i--;
		}
	}
	if (i == 0)
		return (-1);
	else
		return (0);
}

void	close_unused_pipes(t_pipe_tab *pipe_tab, int pos)
{
	int	i;

	i = pos - 1;
	while (i > 0)
	{
		close(pipe_tab->pipes[i * 2 + READ]);
		close(pipe_tab->pipes[i * 2 + WRITE]);
		i--;
	}
	if (pipe_tab->pipes[pos * 2 + WRITE] != NOT_PIPE)
		close(pipe_tab->pipes[pos * 2 + WRITE]);
	if (pipe_tab->pipes[(pos + 1) * 2 + READ] != NOT_PIPE)
		close(pipe_tab->pipes[(pos + 1) * 2 + READ]);
	if (pos != 0)
		close(pipe_tab->pipes[0 + READ]);
	if (pos != pipe_tab->len - 2)
		close(pipe_tab->pipes[(pipe_tab->len - 1) * 2 + WRITE]);
	i = pos + 2;
	while (i < pipe_tab->len - 1)
	{
		close(pipe_tab->pipes[i * 2 + READ]);
		close(pipe_tab->pipes[i * 2 + WRITE]);
		i++;
	}
}
