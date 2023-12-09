/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:49:27 by bplante           #+#    #+#             */
/*   Updated: 2023/11/22 15:09:56 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	run(char **argv, int io_files[2], char **env);

int	main(int argc, char **argv, char **env)
{
	int	io_files[2];
	int	status;

	if (argc < 5)
	{
		ft_printf_fd(
			"pipex: usage <in_file> \"<command>\" \"<command>\" <out_file>\n",
			2);
		return (1);
	}
	if (check_io_files(argv[1], argv[argc - 1], io_files) == 0)
	{
		status = run(&argv[2], io_files, env);
		close(io_files[1]);
		close(io_files[0]);
		return (status);
	}
	return (1);
}

int	loop(char **argv, int io_files[2], char **env, int fd_pipe1[2])
{
	int			fd_pipe2[2];
	t_pipe_pair	*pipe_pair;
	int			status;

	while (argv[2])
	{
		pipe(fd_pipe2);
		close(fd_pipe1[1]);
		pipe_pair = create_pipe_pair(fd_pipe1[0], fd_pipe1[1], fd_pipe2[0],
				fd_pipe2[1]);
		status = create_children(argv[0], env, pipe_pair, io_files);
		free(pipe_pair);
		if (status != 0)
		{
			close_pipes(fd_pipe1, fd_pipe2);
			return (status);
		}
		switch_close_pipes(fd_pipe1, fd_pipe2);
		argv++;
	}
	close(fd_pipe1[1]);
	return (0);
}

int	run(char **argv, int io_files[2], char **env)
{
	t_pipe_pair	*pipe_pair;
	int			fd_pipe[2];
	int			status;

	if (pipe(fd_pipe) != 0)
	{
		ft_printf_fd("pipex: pipe: %s\n", 2, strerror(errno));
		return (-1);
	}
	status = first_children(argv, env, io_files, fd_pipe);
	if (status != 0)
		return (status);
	status = loop(&argv[1], io_files, env, fd_pipe);
	if (status != 0)
		return (status);
	pipe_pair = create_pipe_pair(fd_pipe[0], fd_pipe[1], io_files[0],
			io_files[1]);
	status = create_children(argv[1], env, pipe_pair, NULL);
	close_pipes(fd_pipe, NULL);
	free(pipe_pair);
	return (status);
}

void	children(char **exec, char **env, t_pipe_pair *pipe_pair,
		int extra_close[2])
{
	if (dup_close_pipes(pipe_pair, extra_close) == -1)
	{
		ft_printf_fd("pipex: piping error: %s\n", 2, strerror(errno));
	}
	else
	{
		execve(exec[0], exec, env);
		ft_printf_fd("pipex: %s: %s\n", 2, strerror(errno), exec[0]);
	}
}

int	parent(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	return (status / 256);
}
