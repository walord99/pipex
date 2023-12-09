/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handling.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 17:45:32 by bplante           #+#    #+#             */
/*   Updated: 2023/12/03 17:28:46 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int			check_file(char *file, int io);

int	dup_close_pipes(t_pipe_pair *pipe_pair, int extra_close[2])
{
	if (pipe_pair->read[1] != -1)
	{
		close(pipe_pair->read[1]);
	}
	if (pipe_pair->write[0] != -1)
	{
		close(pipe_pair->write[0]);
	}
	if (dup2(pipe_pair->read[0], 0) == -1)
		return (-1);
	if (dup2(pipe_pair->write[1], 1) == -1)
		return (-1);
	close(pipe_pair->read[0]);
	close(pipe_pair->write[1]);
	if (extra_close)
	{
		close(extra_close[1]);
		close(extra_close[0]);
	}
	return (0);
}

int	check_file(char *file, int io)
{
	int	fd;

	if (io == IN_FILE)
		fd = open(file, O_RDONLY);
	else if (io == OUT_OVER)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (io == OUT_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = -1;
	if (fd == -1)
	{
		ft_printf_fd("pipex: %s: %s\n", 2, strerror(errno), file);
	}
	return (fd);
}

int	check_io_files(char *in, char *out, int fd[2])
{
	fd[0] = check_file(in, IN_FILE);
	fd[1] = check_file(out, OUT_OVER);
	if (fd[0] == -1 || fd[1] == -1)
	{
		if (fd[0] == -1 && fd[1] >= 0)
			close(fd[1]);
		if (fd[1] == -1 && fd[0] >= 0)
			close(fd[0]);
		return (-1);
	}
	return (0);
}

void	close_pipes(int fd_pipe[2], int fd[2])
{
	if (fd)
	{
		close(fd[1]);
		close(fd[0]);
	}
	close(fd_pipe[0]);
	close(fd_pipe[1]);
}

t_pipe_pair	*create_pipe_pair(int read0, int read1, int write0, int write1)
{
	t_pipe_pair	*pp;

	pp = ft_calloc(sizeof(t_pipe_pair), 1);
	pp->read[0] = read0;
	pp->read[1] = read1;
	pp->write[0] = write0;
	pp->write[1] = write1;
	return (pp);
}
