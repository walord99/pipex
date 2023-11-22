/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_spawner.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:55:25 by bplante           #+#    #+#             */
/*   Updated: 2023/11/22 15:04:25 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	first_children(char **argv, char **env, int io_files[2], int fd_pipe[0])
{
	t_pipe_pair	*pipe_pair;
	int			status;

	pipe_pair = create_pipe_pair(io_files[0], io_files[1], fd_pipe[0],
			fd_pipe[1]);
	status = create_children(argv[0], env, pipe_pair, NULL);
	free(pipe_pair);
	if (status != 0)
		close_pipes(fd_pipe, NULL);
	return (status);
}

int	create_children(char *exec, char **env, t_pipe_pair *pipe_pair,
		int extra_close[2])
{
	pid_t	pid;
	char	**split;

	pid = fork();
	if (pid < 0)
		ft_printf_fd("pipex: failed to fork: %s\n", 2, strerror(errno));
	else if (pid == 0)
	{
		split = split_args(exec);
		if (check_exec(split, env) == 0)
			children(split, env, pipe_pair, extra_close);
		free_tab((void **)split, &free);
		exit(1);
	}
	else
	{
		return (parent(pid));
	}
	return (1);
}

void	switch_close_pipes(int fd1[2], int fd2[2])
{
	int	temp[2];

	temp[0] = fd1[0];
	temp[1] = fd1[1];
	fd1[0] = fd2[0];
	fd1[1] = fd2[1];
	fd2[0] = temp[0];
	fd2[1] = temp[1];
	close(fd2[0]);
}
