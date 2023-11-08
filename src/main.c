/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:49:27 by bplante           #+#    #+#             */
/*   Updated: 2023/11/08 03:10:06 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
/*
int main(int argc, char **argv)
{
	pid_t pid;
	pid = fork();
    if (pid < 0)
        perror("Failed to fork.");
    else if (pid == 0)1
    {
		char *t[] = {NULL};
		char *test[] = {"/usr/bin/grep", "-R", "src", "src", NULL};
		execve(test[0], test, t);
	}
	else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status))
        {
            int exit_status = WEXITSTATUS(status);
            printf("Parent: Process ID %ld Exit status of the child was %d\n", (long)getpid, exit_status);
        }
    }
    return 0;
}
*/

int main(int argc, char **argv)
{
    char **call1 = ft_split(argv[2], ' ');
    char *file = argv[1];
    int fd_in = open(file, O_RDONLY);
    //int fd_out = open("argv[2]");
    dup2(fd_in, 0);
    close(fd_in);

    pid_t pid;
    pid = fork();
    if (pid < 0)
        perror("Failed to fork");
    else if (pid == 0)
    {
        call1[0] = ft_strjoin("/usr/bin/", call1[0]);
        execve(call1[0], call1, NULL);
    }
    else 
    {
        int status;
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status))
        {
            int exit_status = WEXITSTATUS(status);
            printf("Parent: Process ID %ld Exit status of the child was %d\n", (long)getpid, exit_status);
        }
    }

}
