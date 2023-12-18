/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 00:04:31 by bplante/Wal       #+#    #+#             */
/*   Updated: 2023/12/18 14:54:10 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	wait_childs(int *pids);
static int	create_childs(char **cmds, t_pipe_tab *pipe_tab, int *pids,
				char **env);
static void	exec(char **cmd, char **env, t_pipe_tab *pipe_tab, int pos);

int	main(int argc, char **argv, char **env)
{
	t_pipe_tab	pipe_tab;
	int			*pids;
	int			status;
	int			file_stat;

	if (argc < 5)
	{
		ft_printf_fd("usage: file command command file\n", 2);
		return (1);
	}
	file_stat = create_pipes_tab(&pipe_tab, argc, argv);
	if (file_stat == 2)
		return (1);
	pids = ft_calloc(argc - 3 + 1, sizeof(int));
	create_childs(&argv[2], &pipe_tab, pids, env);
	close_all_pipes(&pipe_tab);
	status = wait_childs(pids);
	free(pipe_tab.pipes);
	free(pids);
	if (file_stat == 0)
		return (status);
	return (file_stat);
}

int	wait_childs(int *pids)
{
	int	i;
	int	status;

	i = 0;
	while (pids[i])
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	return (status / 256);
}

int	create_childs(char **cmds, t_pipe_tab *pipe_tab, int *pids, char **env)
{
	int		i;
	char	**cmd_arg;
	int		status;

	i = 0;
	while (i < pipe_tab->len - 1)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			cmd_arg = split_args(cmds[i]);
			status = check_exec(cmd_arg, env);
			if (status == 0)
				exec(cmd_arg, env, pipe_tab, i);
			close_all_pipes(pipe_tab);
			free(pipe_tab->pipes);
			free_tab((void **)cmd_arg, &free);
			free(pids);
			exit(status);
		}
		i++;
	}
	return (0);
}

void	exec(char **cmd, char **env, t_pipe_tab *pipe_tab, int pos)
{
	close_unused_pipes(pipe_tab, pos);
	dup2(pipe_tab->pipes[pos * 2 + READ], 0);
	dup2(pipe_tab->pipes[(pos + 1) * 2 + WRITE], 1);
	close(pipe_tab->pipes[pos * 2 + READ]);
	close(pipe_tab->pipes[(pos + 1) * 2 + WRITE]);
	execve(cmd[0], cmd, env);
}
