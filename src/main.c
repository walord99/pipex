/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante/Walord <benplante99@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 00:04:31 by bplante/Wal       #+#    #+#             */
/*   Updated: 2023/12/11 04:30:20 by bplante/Wal      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	wait_childs(int *pids);
static int	create_childs(char **cmds, t_pipe_tab *pipe_tab, int *pids,
				char **env);
static void	exec(char **cmd, char **env, t_pipe_tab *pipe_tab, int pos);

int	main(int argc, char **argv, char **env)
{
	t_pipe_tab	pipe_tab;
	int			*pids;
	int test;

	if (argc > 5)
		return (1);
	if (create_pipes_tab(&pipe_tab, argc, argv))
		return (1);
	pids = ft_calloc(argc - 3 + 1, sizeof(int));
	create_childs(&argv[2], &pipe_tab, pids, env);
	close_all_pipes(&pipe_tab);
	wait_childs(pids);
	free(pipe_tab.pipes);
	free(pids);
}

void	wait_childs(int *pids)
{
	int	i;

	i = 0;
	while (pids[i])
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

int	create_childs(char **cmds, t_pipe_tab *pipe_tab, int *pids, char **env)
{
	int		i;
	char	**cmd_arg;

	i = 0;
	while (i < pipe_tab->len - 1)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			sleep(8);
			cmd_arg = split_args(cmds[i]);
			if (check_exec(cmd_arg, env) == 0)
				exec(cmd_arg, env, pipe_tab, i);
			close_all_pipes(pipe_tab);
			free(pipe_tab->pipes);
			free_tab((void **)cmd_arg, &free);
			exit(1);
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
