/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:49:27 by bplante           #+#    #+#             */
/*   Updated: 2023/11/08 13:58:51 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
int	main(int argc, char **argv)
{
	pid_t	pid;
		char *t[] = {NULL};
		char *test[] = {"/usr/bin/grep", "-R", "src", "src", NULL};
		int status;
	int		exit_status;

	pid = fork();
	if (pid < 0)
		perror("Failed to fork.");
	else if (pid == 0)1
	{
		execve(test[0], test, t);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status))
		{
			exit_status = WEXITSTATUS(status);
			printf("Parent: Process ID %ld Exit status of the child was %d\n",
				(long)getpid, exit_status);
		}
	}
	return (0);
}
*/

int	main(int argc, char **argv)
{
	char	*file;
	int		fd_in;
	int		fd_out;
	int		fd_pipe1[2];
	int		fd_pipe2[2];
	int		loops;
	char	**call;
	pid_t	pid;
	int		status;

	file = argv[1];
	fd_in = open(file, O_RDONLY);
	file = argv[4];
	fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	pipe(fd_pipe2);
	pipe(fd_pipe1);
	dup2(fd_in, 0);
	loops = argc - 3;
	while (loops--)
	{
		if (loops % 2 == 1)
		{
			if (loops != argc - 4)
				dup2(fd_pipe2[0], 0);
			if (loops == 0)
				dup2(fd_out, 1);
			else
				dup2(fd_pipe1[1], 1);
		}
		else
		{
			if (loops != argc - 4)
				dup2(fd_pipe1[0], 0);
			if (loops == 0)
				dup2(fd_out, 1);
			else
			 	dup2(fd_pipe2[1], 1);
		}
		call = ft_split(argv[argc - 2 - loops], ' ');
		pid = fork();
		if (pid < 0)
			perror("Failed to fork");
		else if (pid == 0)
		{
			call[0] = ft_strjoin("/usr/bin/", call[0]);
			execve(call[0], call, NULL);
		}
		else
		{
			// free split
			waitpid(-1, &status, 0);
		}
	}
}
