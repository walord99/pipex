/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:01:17 by bplante           #+#    #+#             */
/*   Updated: 2023/11/21 08:53:58 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	run(char **argv, int io_files[2], char **env);
static int	create_children(char *exec, char **env, t_pipe_pair *pipe_pair,
				int extra_close[2]);
static int	parent(pid_t pid);
static void	children(char **exec, char **env, t_pipe_pair *pipe_pair,
				int extra_close[2]);

int	main(int argc, char **argv, char **env)
{
	int	io_files[2];
	int	status;

	if (argc != 5)
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
	pipe_pair = create_pipe_pair(io_files[0], io_files[1], fd_pipe[0],
			fd_pipe[1]);
	status = create_children(argv[0], env, pipe_pair, NULL);
	if (status != 0)
	{
		close_and_free(fd_pipe, pipe_pair);
		return (status);
	}
	free(pipe_pair);
	pipe_pair = create_pipe_pair(fd_pipe[0], fd_pipe[1], io_files[0],
			io_files[1]);
	status = create_children(argv[1], env, pipe_pair, NULL);
	close_and_free(fd_pipe, pipe_pair);
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
		close(pipe_pair->write[1]);
		return (parent(pid));
	}
	return (1);
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
