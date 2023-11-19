/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:01:17 by bplante           #+#    #+#             */
/*   Updated: 2023/11/19 17:41:36 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#define IN_FILE 0
#define OUT_OVER 1
#define OUT_APPEND 2

typedef struct s_pipe_pair
{
	int		read[2];
	int		write[2];
}			t_pipe_pair;

static int	check_exec(char **exec, char **env);

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
		ft_printf_fd("%s: %i\n", 2, __FILE__, __LINE__ - 1);
	}
}

int	parent(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	return (status / 256);
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
		split = ft_split(exec, ' ');
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

int	check_file(char *file, int io)
{
	int	fd;

	if (io == IN_FILE)
		fd = open(file, O_RDONLY);
	else if (io == OUT_OVER)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (io == OUT_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_printf_fd("pipex: %s: %s\n", 2, strerror(errno), file);
	}
	return (fd);
}

char	*get_path(char **env, char **exec)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	paths = ft_split(get_env(env, "PATH"), ':');
	while (paths[i])
	{
		path = ft_strjoin_free(paths[i], ft_strjoin("/", exec[0]), FREE_S2);
		if (access(path, F_OK) == 0)
		{
			free_tab((void **)paths, &free);
			return (path);
		}
		free(path);
		i++;
	}
	free_tab((void **)paths, &free);
	return (NULL);
}

int	check_exec(char **exec, char **env)
{
	char	*ptr;

	if (!exec)
		return (1);
	if (!ft_strchr(exec[0], '/'))
	{
		ptr = exec[0];
		exec[0] = get_path(env, exec);
		free(ptr);
	}
	if (exec[0] && access(exec[0], X_OK) != 0)
	{
		ft_printf_fd("pipex: %s: %s\n", 2, strerror(errno), exec[0]);
		return (-11);
	}
	else if (!exec[0])
	{
		ft_printf_fd("pipex: command not found: %s\n", 2, exec[0]);
		return (-1);
	}
	return (0);
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

// int	check_all_exec(char ***execs, char **env)
// {
// 	int	i;
// 	int	ret;

// 	i = 0;
// 	while (execs[i])
// 	{
// 		ret = check_exec(execs[i], env) != 0);
// 		if (ret != 0)
// 			return (ret);
// 		i++;
// 	}
// 	return (0);
// }

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
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		free(pipe_pair);
		return (status);
	}
	free(pipe_pair);
	pipe_pair = create_pipe_pair(fd_pipe[0], fd_pipe[1], io_files[0],
			io_files[1]);
	status = create_children(argv[1], env, pipe_pair, NULL);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	free(pipe_pair);
	return (status);
}

int	main(int argc, char **argv, char **env)
{
	int	io_files[2];
	int	status;

	if (argc != 5)
	{
		ft_printf_fd("pipex: usage <in_file> \"<command>\" \"<command>\" <out_file>\n",
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
